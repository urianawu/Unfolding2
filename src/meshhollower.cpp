#include "meshhollower.h"
#include "MeshExtender.h"


double MeshHollower::flapSize = 0.2;//[0 - 1]
double MeshHollower::shiftAmount = 0;//[-1 - 1]

HDS_Mesh* MeshHollower::thismesh = nullptr;
vector<HDS_Vertex*> MeshHollower::vertices_new;
vector<HDS_HalfEdge*> MeshHollower::hes_new;

void MeshHollower::hollowMesh(HDS_Mesh* mesh, double newFlapSize, int type, double shift)
{
	/*ignore cut edges*/
	flapSize = newFlapSize;//Flap size needed in export function
	thismesh = mesh;
	shiftAmount = shift;

	typedef HDS_HalfEdge he_t;
	typedef HDS_Vertex vert_t;
	typedef HDS_Face face_t;

	vector<face_t*> old_faces;
	unordered_set<he_t*> old_edges;
	for (auto he: thismesh->heSet) {
		if (old_edges.find(he->flip) == old_edges.end())
			old_edges.insert(he);
	}

	for (auto f : thismesh->faceSet) {
		f->setScaleFactor(HDS_Bridger::getScale());
		old_faces.push_back(f);
	}
	for (auto f: old_faces) {
		thismesh->deleteFace(f);
	}
	//clear old mesh
	thismesh->vertSet.clear();
	thismesh->vertMap.clear();
	for (auto he: old_edges) {
		thismesh->deleteHalfEdge(he);
		thismesh->deleteHalfEdge(he->flip);
	}

	HDS_Vertex::resetIndex();
	HDS_HalfEdge::resetIndex();
	HDS_Face::resetIndex();


	//set new bridger on each edge
	for (auto he : old_edges) {
		cout<<"bridger based on original edge "<<he->index<<endl;
		//get edge vertex, calculate scaled vertex
		vert_t* he_v = he->v;
		vert_t* he_flip_v = he->flip->v;
		face_t* he_f = he->f;
		face_t* he_flip_f = he->flip->f;

		vert_t* he1_v1 = new vert_t(he_f->scaleCorner(he_v));
		vert_t* he1_v2 = new vert_t(he_f->scaleCorner(he_flip_v));
		vert_t* he2_v1 = new vert_t(he_flip_f->scaleCorner(he_v));
		vert_t* he2_v2 = new vert_t(he_flip_f->scaleCorner(he_flip_v));

		// Assign refid
		he1_v1->refid = he2_v1->refid
			= HDS_Common::assignRef_ID(he_v->index, HDS_Common::FROM_VERTEX);
		he1_v2->refid = he2_v2->refid
			= HDS_Common::assignRef_ID(he_flip_v->index, HDS_Common::FROM_VERTEX);

		vertices_new.push_back(he1_v1);
		vertices_new.push_back(he1_v2);
		vertices_new.push_back(he2_v1);
		vertices_new.push_back(he2_v2);

		//new edge pair based on new vertex position
		he_t* he1 = thismesh->insertEdge(he1_v1, he1_v2);
		he_t* he2 = thismesh->insertEdge(he2_v2, he2_v1);

		// Assign refid for edges
		// *	      *
		// ||		/ || \
		// ||	->	| || |
		// ||		\ || /
		// *		  *
		he1->refid = he1->flip->refid
			= HDS_Common::assignRef_ID(he->index, HDS_Common::FROM_EDGE);
		he2->refid = he2->flip->refid
			= HDS_Common::assignRef_ID(he->flip->index, HDS_Common::FROM_EDGE);

		hes_new.push_back(he1);
		hes_new.push_back(he1->flip);
		hes_new.push_back(he2);
		hes_new.push_back(he2->flip);

		//set edge cut face
		face_t * cutFace = new face_t;
		cutFace->index = HDS_Face::assignIndex();
		cutFace->isCutFace = true;
		cutFace->he = he1;
		he1->setCutEdge(true);
		he2->f = cutFace;
		he2->setCutEdge(true);
		thismesh->addFace(cutFace);

		//add bridger
		he1->f = he_f;//pass original face to addBridger function

		vector<vert_t*> verts = MeshExtender::addBridger(thismesh, he1->flip, he2->flip, cutFace);
		vertices_new.insert( vertices_new.end(), verts.begin(), verts.end() );
		he1->f = cutFace;

		if (type != 2 && flapSize < 0.01)
		{
			continue;
		}
		else
		{
			he1->setCutEdge(false);
			he2->setCutEdge(false);

			thismesh->addFace(addFlapFace(type, he, he1, cutFace));
			thismesh->addFace(addFlapFace(type, he->flip, he2, cutFace));
		}
	}

	//add new vertices and edges
	for (auto v: vertices_new) {
		v->index = HDS_Vertex::assignIndex();
		thismesh->addVertex(v);
	}
	for (auto he: hes_new) {
		he->index = HDS_HalfEdge::assignIndex();
		thismesh->addHalfEdge(he);
	}
	/// update the curvature of each vertex
	for (auto &v : thismesh->vertSet) {
		v->computeNormal();
		v->computeCurvature();
		//cout << v->index << ": " << (*v) << endl;
	}


	// Set mark for hollowed mesh
	thismesh->updatePieceSet();
}

HDS_Face* MeshHollower::addFlapFace(int type, HDS_HalfEdge* originalHE, HDS_HalfEdge* startHE, HDS_Face* cutFace )
{
	vector<QVector3D> vertPos;
	HDS_Face* he_f = originalHE->f;

	QVector3D v0 = he_f->scaleCorner(originalHE->prev->v);
	QVector3D v3 = he_f->scaleCorner(originalHE->next->flip->v);

	QVector3D v2_flap = (1.0 - flapSize) * startHE->flip->v->pos + flapSize * v3;
	QVector3D v1_flap = (1.0 - flapSize) * startHE->v->pos + flapSize * v0;

	vector<HDS_Vertex*> vertices;//kkkkkkkkkkkkk
	vertices.push_back(startHE->flip->v);
	switch(type)
	{
	case 0://one flap
	{
		thismesh->processType = HDS_Mesh::HOLLOWED_PROC;

		vertPos.push_back(v2_flap);
		vertPos.push_back(v1_flap);
		break;
	}
	case 1://mult flap
	{
		thismesh->processType = HDS_Mesh::HOLLOWED_PROC;

		QVector3D v0_prev = he_f->scaleCorner(originalHE->prev->prev->v);
		QVector3D v3_prev = he_f->scaleCorner(originalHE->next->next->flip->v);

		QVector3D v0_flap = (1.0 - flapSize)*v0 + flapSize*v0_prev;
		QVector3D v3_flap = (1.0 - flapSize)*v3 + flapSize*v3_prev;

		QVector3D v2_flap_flap = (1.0 - flapSize)*v2_flap + flapSize*v1_flap;
		QVector3D v1_flap_flap = (1.0 - flapSize)*v1_flap + flapSize*v2_flap;

		float right_scale = (shiftAmount + 1)/2;
		float left_scale = 1 - right_scale;

		if(shiftAmount != -1)
		{
			//right flap
			QVector3D v3_scaled = (1.0 - right_scale)*v2_flap + right_scale*v3;
			QVector3D v3_flap_scaled = (1.0 - right_scale)*v2_flap_flap + right_scale*v3_flap;
			vertPos.push_back(v3_scaled);
			vertPos.push_back(v3_flap_scaled);
			vertPos.push_back(v2_flap_flap);
		}
		else
		{
			vertPos.push_back(v2_flap);
		}

		if(shiftAmount != 1)
		{
			//left flap
			QVector3D v0_scaled = (1.0-left_scale)*v1_flap + left_scale*v0;
			QVector3D v0_flap_scaled = (1.0-left_scale)*v1_flap_flap + left_scale*v0_flap;
			vertPos.push_back(v1_flap_flap);
			vertPos.push_back(v0_flap_scaled);
			vertPos.push_back(v0_scaled);
		}
		else
		{
			vertPos.push_back(v1_flap);
		}

		break;
	}
	case 2://bind
	{
		thismesh->processType = HDS_Mesh::BINDED_PROC;
		auto curHE = originalHE->next;
		do {
			vertPos.push_back(he_f->scaleCorner(curHE->flip->v));
			curHE = curHE->next;
		} while (curHE != originalHE->prev);
		break;
	}
	default:
		break;
	}
	vertices.push_back(startHE->v);//kkkkkkkkkkkkk

	HDS_Face * newFace = createFace(startHE->flip->v, vertPos, startHE->v, cutFace);
	//HDS_Face* newFace = createFace(vertices, cutFace);//kkkkkkkkkkkkk
	newFace->he = startHE;
	startHE->f = newFace;

	return newFace;
}

//vertPos: [new verts..]
HDS_Face* MeshHollower::createFace(
	HDS_Vertex* startV, vector<QVector3D> vertPos, HDS_Vertex* endV, HDS_Face* cutFace)
{
	typedef HDS_HalfEdge he_t;
	typedef HDS_Vertex vert_t;
	typedef HDS_Face face_t;

	face_t * newFace = new face_t;
	newFace->index = HDS_Face::assignIndex();

	vert_t* curV = startV;
	for (auto v : vertPos)
	{
		vert_t* newV = new vert_t(v);
		vertices_new.push_back(newV);

		he_t* newHE = thismesh->insertEdge(curV, newV);
		newHE->f = newFace;
		newHE->flip->f = cutFace;
		newHE->setCutEdge(true);
		hes_new.push_back(newHE);
		hes_new.push_back(newHE->flip);
		curV = newV;
	}

	//link last edge of the face
	he_t* lastHE = thismesh->insertEdge(curV, endV);
	lastHE->f = newFace;
	lastHE->flip->f = cutFace;
	lastHE->setCutEdge(true);
	hes_new.push_back(lastHE);
	hes_new.push_back(lastHE->flip);

	return newFace;
}

HDS_Face* MeshHollower::createFace(vector<HDS_Vertex*> vertices, HDS_Face* cutFace)
{
	typedef HDS_HalfEdge he_t;
	typedef HDS_Vertex vert_t;
	typedef HDS_Face face_t;

	face_t * newFace = new face_t;
	newFace->index = HDS_Face::assignIndex();

	auto preV = vertices.front();
	for (int i = 1; i < vertices.size() - 1; i++)
	{
		auto& curV = vertices[i];
		vertices_new.push_back(curV);
		he_t* newHE = thismesh->insertEdge(preV, curV);
		newHE->f = newFace;
		newHE->flip->f = cutFace;
		newHE->setCutEdge(true);
		hes_new.push_back(newHE);
		hes_new.push_back(newHE->flip);
		preV = curV;

	}

	//link last edge of the face
	he_t* lastHE = thismesh->insertEdge(preV, vertices.back());
	lastHE->f = newFace;
	lastHE->flip->f = cutFace;
	lastHE->setCutEdge(true);
	hes_new.push_back(lastHE);
	hes_new.push_back(lastHE->flip);

	return newFace;
}
