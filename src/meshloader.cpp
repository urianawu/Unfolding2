#include "meshloader.h"
#include "stringutils.h"

#include <QFile>
#include <QString>
#include <QTextStream>


void MeshLoader::clear() {
	/*verts.clear();
	faces.clear();
	texcoords.clear();
	normals.clear();

	verts.reserve(131076);
	normals.reserve(131076);
	texcoords.reserve(131076);
	faces.reserve(131076*2);*/
	vertices.clear();
	uvs.clear();
	normals.clear();
	for (auto poly : polys)
	{
		delete poly;
	}
	polys.clear();
}
/*
void MeshLoader::triangulate(){
	cout << "Triangulating the mesh ..." << endl;
	vector<face_t> newFaces;

	for(size_t i=0;i<faces.size();i++)
	{
		const face_t& Fi = faces[i];

		if( Fi.v.size() > 3 )
		{
			// triangulate this face

			for(size_t j=1;j<Fi.v.size()-1;j++)
			{
				face_t f;
				f.v.push_back(Fi.v[0]);
				f.v.push_back(Fi.v[j]);
				f.v.push_back(Fi.v[j+1]);

				f.t.push_back(Fi.t[0]);
				f.t.push_back(Fi.t[j]);
				f.t.push_back(Fi.t[j+1]);

				f.n.push_back(Fi.n[0]);
				f.n.push_back(Fi.n[j]);
				f.n.push_back(Fi.n[j+1]);

				newFaces.push_back(f);
			}
		}
		else
		{
			newFaces.push_back(Fi);
		}
	}

	faces = newFaces;

	triangulated = true;

	hasVertexNormal = false;
	hasVertexTexCoord = false;

	cout << "done.";
}

void MeshLoader::estimateNormals()
{
	if( hasVertexNormal )
	{
		cout << "already has vertex normal ..." << endl;
		// only estimate face normal
		for(size_t i=0;i<faces.size();i++)
		{
			QVector3D p0, p1, p2;

			p0 = verts[faces[i].v[0]];
			p1 = verts[faces[i].v[1]];
			p2 = verts[faces[i].v[2]];

			norm_t n = QVector3D::crossProduct(p0 - p1, p2 - p1);
			n.normalize();

			faces[i].normal = n;
		}
	}
	else
	{
		normals.resize(verts.size());
		for(size_t i=0;i<verts.size();i++)
			normals[i] = norm_t(0, 0, 0);

		// for each face, compute its normal
		// add the contribution to its vertices
		for(size_t i=0;i<faces.size();i++)
		{

			QVector3D p0, p1, p2;

			p0 = verts[faces[i].v[0]];
			p1 = verts[faces[i].v[1]];
			p2 = verts[faces[i].v[2]];

			norm_t n = QVector3D::crossProduct(p0 - p1, p2 - p1);
			n.normalize();

			faces[i].normal = n;

			for(size_t j=0;j<faces[i].v.size();j++)
			{
				int pidx, nidx, idx;
				idx = j;
				pidx = j-1;
				if( pidx < 0 ) pidx += faces[i].v.size();
				nidx = j+1;
				if( nidx > faces[i].v.size() - 1 ) nidx -= faces[i].v.size();

				QVector3D vp, vc, vn;
				vp = verts[faces[i].v[pidx]];
				vc = verts[faces[i].v[idx]];
				vn = verts[faces[i].v[nidx]];

				QVector3D e1 = vp - vc, e2 = vn - vc;

				float theta = QVector3D::dotProduct(e1, e2) / (e1.length() * e2.length());

				normals[faces[i].v[idx]] += theta * n;
			}
		}

		for(size_t i=0;i<normals.size();i++)
			normals[i].normalize();
	}
}
*/
istream& operator>>(istream& is, QVector3D& v) {
	qreal x, y, z;
	is >> x >> y >> z;
	v.setX(x); v.setY(y); v.setZ(z);
	return is;
}

istream& operator>>(istream& is, QVector2D& v) {
	qreal x, y;
	is >> x >> y;
	v.setX(x); v.setY(y);
	return is;
}

bool OBJLoader::load(const string &filename)
{
	try{
		if (filename[0] == ':')// resource file
		{
			cout << "Using file from qrc\n";
			load_from_string(filename);
		}
		else
		{
			load_from_file(filename);
		}
		/*QFile file(filename.c_str());
		if (!file.open(QFile::ReadOnly | QFile::Text)) {
			cerr << "Failed to open file " << filename << endl;
			return false;
		}
		int fileHandle = file.handle();
		if (fileHandle == -1)
		{
			return false;
		}*/
		

		cout << "finish loading file " << filename << endl;
		cout << "# faces = " << polys.size() << endl;
		cout << "# vertices = " << vertices.size() << endl;

		return true;
	}
	catch( exception e )
	{
		cerr << e.what() << endl;
		return false;
	}
}

OBJLoader::index_t OBJLoader::facetype(const char * str, uint32_t * val)
{
	int argv = sscanf(str, "%d/%d/%d", val, val + 1, val + 2);
	switch (argv)
	{
	case 3:// V/T/N
		return VTN;//111
	case 2:// V/T
		return VT;//011
	case 1:
		argv = sscanf(str, "%d//%d", val, val + 2);
		if (argv == 2)// V//N
		{
			return VN;//101
		}
		else// V
		{
			return V;//001
		}
	}
}

bool OBJLoader::load_from_string(const string &filename)
{
	QFile f(filename.c_str());
	if (!f.open(QFile::ReadOnly))
		return false;
	auto str = QTextStream(&f).readAll().toUtf8().toStdString();
	const char* srcStr = str.c_str();
	f.close();
	cout << "finish reading file\n";
	if (srcStr != nullptr)
	{
		auto subStr = srcStr;
		int err;
		int offset = 0;
		char trash[255] = {};
		char lineHeader[2] = {};
		float val[3] = {};
		uint32_t indices[3];

		do
		{
			// Skip end of line
			if (*subStr == '\n' || *subStr == '\r')
			{
				subStr++;
				continue;
			}
			lineHeader[0] = lineHeader[1] = 0;
			sscanf(subStr, "%s%n", &lineHeader, &offset);
			subStr += offset;
			// Vertex
			if (strcmp(lineHeader, "v") == 0)
			{
				sscanf(subStr, "%f %f %f%n", val, val + 1, val + 2, &offset);
				vertices.insert(vertices.end(), val, val + 3);

				subStr += offset + 1;
			}
			// Texture Coordinate
			else if (strcmp(lineHeader, "vt") == 0)
			{
				err = sscanf(subStr, "%f %f%n", val, val + 1, &offset); uvs.insert(uvs.end(), val, val + 2);
				subStr += offset + 1;
			}
			// Vertex Normal
			else if (strcmp(lineHeader, "vn") == 0)
			{
				//float val[3];
				err = sscanf(subStr, "%f %f %f%n", val, val + 1, val + 2, &offset);
				normals.insert(normals.end(), val, val + 3);

				subStr += offset + 1;
			}
			// Face Index
			else if (strcmp(lineHeader, "f") == 0)
			{
				cout << "loading face\n";
				PolyIndex* fid = new PolyIndex;
				err = sscanf(subStr, "%s%n", &trash, &offset);
				indices[1] = indices[2] = 0;
				index_t ft = facetype(trash, indices);
				fid->push_back(indices);
				subStr += offset;

				switch (ft)
				{
				case VTN://111
					while (*subStr != '\n' && *subStr != '\r' && *subStr != '\0')
					{
						err = sscanf(subStr, "%s%n", &trash, &offset);
						sscanf(trash, "%d/%d/%d", indices, indices + 1, indices + 2);
						fid->push_back(indices);
						subStr += offset;
					}
					break;
				case VT://011
					while (*subStr != '\n' && *subStr != '\r' && *subStr != '\0')
					{
						err = sscanf(subStr, "%s%n", &trash, &offset);
						sscanf(trash, "%d/%d", indices, indices + 1);
						fid->push_back(indices);
						subStr += offset;
					}
					break;
				case VN://101
					while (*subStr != '\n' && *subStr != '\r' && *subStr != '\0')
					{
						err = sscanf(subStr, "%s%n", &trash, &offset);
						sscanf(trash, "%d//%d", indices, indices + 2);
						fid->push_back(indices);
						subStr += offset;
					}
					break;
				case V://001
					while (*subStr != '\n' && *subStr != '\r' && *subStr != '\0')
					{
						err = sscanf(subStr, "%s%n", &trash, &offset);
						sscanf(trash, "%d", indices);
						fid->push_back(indices);
						subStr += offset;
					}
					break;
				default:
					break;
				}
				fid->size = fid->v.size();
				polys.push_back(fid);
				subStr++;
			}
			// Comment
			else if (strcmp(lineHeader, "#") == 0)
			{
				err = sscanf(subStr, "%[^\r\n]%n", &trash, &offset);
				subStr += offset + 1;
			}
			// Others
			else
			{
				// skip everything except \n and \r
				err = sscanf(subStr, "%[^\r\n]%n", &trash, &offset);
				subStr += offset + 1;
			}

		} while (*subStr != '\0');
		cout << "hehe\n";
		//delete[] srcStr;
	}
}

bool OBJLoader::load_from_file(const string &filename)
{
	FILE* fp = fopen(filename.c_str(), "r");
	if (fp == nullptr)
	{
		return false;
	}
	int err;
	char buff[255] = {};
	char lineHeader[2] = {};
	float val[3] = {};
	uint32_t indices[3];
	char endflg;

	while (true)
	{
		lineHeader[0] = lineHeader[1] = 0;
		err = fscanf(fp, "%2s", &lineHeader);
		if (err == EOF)
		{
			break;
		}
		// Vertex
		if (strcmp(lineHeader, "v") == 0)
		{
			fscanf(fp, "%f %f %f\n", val, val + 1, val + 2);
			vertices.insert(vertices.end(), val, val + 3);
		}
		// Texture Coordinate
		else if (strcmp(lineHeader, "vt") == 0)
		{
			fscanf(fp, "%f %f\n", val, val + 1);
			uvs.insert(uvs.end(), val, val + 2);
		}
		// Vertex Normal
		else if (strcmp(lineHeader, "vn") == 0)
		{
			//float val[3];
			fscanf(fp, "%f %f %f\n", val, val + 1, val + 2);
			normals.insert(normals.end(), val, val + 3);
		}
		// Face Index
		else if (strcmp(lineHeader, "f") == 0)
		{
			//cout << "loading face\n";
			PolyIndex* fid = new PolyIndex;
			err = fscanf(fp, "%s", &buff);
			indices[1] = indices[2] = 0;
			index_t ft = facetype(buff, indices);
			fid->push_back(indices);
			endflg = fgetc(fp);
			switch (ft)
			{
			case VTN://111
				while (endflg != '\n' && endflg != '\r' && endflg != '\0')
				{
					ungetc(endflg, fp);
					fscanf(fp, "%d/%d/%d", indices, indices + 1, indices + 2);
					fid->push_back(indices);
					endflg = fgetc(fp);
				}
				break;
			case VT://011
				while (endflg != '\n' && endflg != '\r' && endflg != '\0')
				{
					ungetc(endflg, fp);
					fscanf(fp, "%d/%d", indices, indices + 1);
					fid->push_back(indices);
					endflg = fgetc(fp);
				}
				break;
			case VN://101
				while (endflg != '\n' && endflg != '\r' && endflg != '\0')
				{
					ungetc(endflg, fp);
					fscanf(fp, "%d//%d", indices, indices + 2);
					fid->push_back(indices);
					endflg = fgetc(fp);
				}
				break;
			case V://001
				while (endflg != '\n' && endflg != '\r' && endflg != EOF)
				{
					ungetc(endflg, fp);
					fscanf(fp, "%d", indices);
					fid->push_back(indices);
					endflg = fgetc(fp);
				}
				break;
			default:
				break;
			}
			fid->size = fid->v.size();
			polys.push_back(fid);
		}
		// Comment
		else if (strcmp(lineHeader, "#") == 0)
		{
			fscanf(fp, "%[^\r\n]", &buff);
		}
		// Others
		else
		{
			// skip everything except \n or \r
			fscanf(fp, "%[^\r\n]", &buff);
		}
	}
	fclose(fp);
}

bool PLYLoader::load(const string& filename) {
	try{
		return true;
	}
	catch( exception e )
	{
		cerr << e.what() << endl;
		return false;
	}
}
