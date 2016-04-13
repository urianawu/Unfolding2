#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "GeodesicComputer.h"
#include "DiscreteGeoComputer.h"

#include "common.h"
#include <QMutex>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QProgressDialog>
#include <QString>
#include <QMessageBox>
#include <QTime>

#include "meshloader.h"
#include "OperationStack.h"

#include "hds_mesh.h"
#include "hds_face.h"
#include "hds_halfedge.h"
#include "hds_vertex.h"
#include "hds_bridger.h"

#include "Graph.hpp"

#define USE_REEB_GRAPH 0
#if USE_REEB_GRAPH
#include <vtkOBJReader.h>
#include <vtkSmartPointer.h>
#endif

class MeshManager
{
public:
	static MeshManager* getInstance()
	{
		static QMutex mutex;
		if (!instance)
		{
			mutex.lock();

			if (!instance)
				instance = new MeshManager;

			mutex.unlock();
		}

		return instance;
	}

	OperationStack* getMeshStack() {
		return operationStack;
	}
	HDS_Mesh* getSmoothedMesh() {
		return smoothed_mesh.data();
	}


#if USE_REEB_GRAPH
	void updateReebGraph(const doubles_t &fvals = doubles_t());
	SimpleGraph* getReebGraph() {
		return &rbGraph;
	}
#endif

	doubles_t getInterpolatedGeodesics(int vidx, int lev0, int lev1, double alpha);
	doubles_t getInterpolatedCurvature(int lev0, int lev1, double alpha);
	doubles_t getInterpolatedPointNormalValue(int lev0, int lev1, double alpha, const QVector3D &pnormal);
	doubles_t getInterpolatedZValue(int lev0, int lev1, double alpha);

	void colorMeshByGeoDistance(int vidx);
	void colorMeshByGeoDistance(int vidx, int lev0, int lev1, double ratio);
	bool saveMeshes();

protected:
	/// should not be externally accessible
	static void drop()
	{
		static QMutex mutex;
		mutex.lock();
		delete instance;
		instance = nullptr;
		mutex.unlock();
	}

private:
	MeshManager() {operationStack = new OperationStack;}
	MeshManager(const MeshManager &) = delete;
	MeshManager& operator=(const MeshManager &) = delete;

	static MeshManager* instance;

	friend class MeshViewerLegacy;
	friend class MeshViewer;
	friend class MeshConnector;

public:

	bool loadOBJFile(const string& filename);
	/*HDS_Mesh* buildHalfEdgeMesh(const vector<MeshLoader::face_t> &faces, const vector<MeshLoader::vert_t> &verts);*/
	HDS_Mesh* buildHalfEdgeMesh(const floats_t &inVerts, const vector<PolyIndex*> &inFaces);
	void cutMeshWithSelectedEdges();
	//void mapToExtendedMesh();
	void unfoldMesh();
	void smoothMesh();

	void setBindMesh();
	void setHollowMesh(double fsize, int type, double shift);
	void extendMesh(map<QString, double> config);
	void rimMesh(double rimSize = 0.0);
	void set3DRimMesh(std::map<QString, bool> config, float width, float height);
	void setWeaveMesh();

	// Export as SVG files
    void exportXMLFile();

	void setCurrentOpFlag(OperationStack::Flag curFlag){operationStack->setCurrentFlag(curFlag);}
private:
	typedef HDS_Mesh mesh_t;
	typedef HDS_HalfEdge he_t;
	typedef HDS_Face face_t;
	typedef HDS_Vertex vert_t;

	OperationStack* operationStack;

	QScopedPointer<HDS_Mesh> smoothed_mesh;
	QScopedPointer<GeodesicComputer> gcomp;
	QScopedPointer<DiscreteGeoComputer> dis_gcomp;


	vector<QSharedPointer<HDS_Mesh>> hds_mesh_smoothed;
	vector<QSharedPointer<GeodesicComputer>> gcomp_smoothed;
#if USE_REEB_GRAPH
	vtkSmartPointer<vtkPolyData> vtkMesh;
	SimpleGraph rbGraph;
#endif
};

#endif // MESHMANAGER_H
