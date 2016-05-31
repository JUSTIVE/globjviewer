#include"Mesh.h"
//#define DEBUG


Mesh::Mesh(){
	this->Init();
}
Mesh::Mesh(const char* filename){
	this->Init();
	this->loadFile(filename);
}
void Mesh::Init(){
	this->prop = 0;
	this->vertexSize = 0;
	this->polygonSize = 0;
	this->polygon_tri = 0;
	this->vertex = NULL;
	this->polygon_tri = NULL;
	this->fileLoading = false;
	this->randColor = false;
	this->drawMode = 2;
	this->polygonShape = TRIANGLE;
	
}
void Mesh::loadFile(string filename) {
	fileLoading = true;
	fileName = filename;
	this->vertexSize = 0;
	this->polygonSize = 0;
	double dummy = 0;
	char dummmy2[256];
#ifdef DEBUG
	cout << "opening file :: " << filename.c_str() << endl;
#endif
	FILE*  fp = fopen(filename.c_str(), "r");
	if (fp == NULL) {
		cout << "file open error :: ";
	}
	this->fileName = filename;
	//peek part
	while (!feof(fp)) {
		fgets(dummmy2, 256, fp);
		if (dummmy2[0] == 'v'&&dummmy2[1] == ' ')
			vertexSize++;
		else if (dummmy2[0] == 'f')
			polygonSize++;
		/*else {
			while (dummmy2 == '\n'){
				fscanf(fp, "%c", &dummmy2);
			}
		}*/
	}
	cout << "COUNT_COMPLETED :: " << endl;
	cout << "vertex :: " << this->vertexSize<< endl;
	cout << "polygon :: " <<this->polygonSize<< endl;
	rewind(fp);
	/*if (vertex != NULL)
		free(vertex);
	if (polygon_tri != NULL)
		free(polygon_tri);*/

	vertex = (V3D*)malloc(sizeof(V3D)*vertexSize);
	if (polygonShape == TRIANGLE)
		polygon_tri = (V3I*)malloc(sizeof(V3I)*polygonSize);
	else
		polygon_quad = (V4I*)malloc(sizeof(V4I)*polygonSize);
	//read part
	int vertexIndex = 0;
	int polygonIndex = 0;
	char chardumm;
	while (!feof(fp)) {
		fgets(dummmy2, 256, fp);
		if (dummmy2[0] == 'v'&&dummmy2[1] == ' '){
			fseek(fp,0-(strlen(dummmy2)+1), SEEK_CUR);
			fscanf(fp, "%c %lf %lf %lf", &chardumm, &vertex[vertexIndex][0], &vertex[vertexIndex][1], &vertex[vertexIndex][2]);
			vertexIndex++;
			if (vertexIndex == vertexSize)
				continue;
		}
		else if (dummmy2[0] == 'f'){
			fseek(fp, 0 - (strlen(dummmy2)+1 ), SEEK_CUR);
			fscanf(fp, "%c %d %d %d", &chardumm, &polygon_tri[polygonIndex][0], &polygon_tri[polygonIndex][1], &polygon_tri[polygonIndex][2]);
			//cout << chardumm << " " << polygon_tri[polygonIndex][0] << polygon_tri[polygonIndex][1] << polygon_tri[polygonIndex][2] << endl;
			polygonIndex++;
			if (polygonIndex == polygonSize)
				break;
		}
		/*else {
			while (dummmy2 == '\n'){
				fscanf(fp, "%c", &dummmy2);
			}
		}*/
		//fscanf(fp, "%c", &chardumm);
	}
	cout << "LOAD_COMPLETED :: " << endl;
	fclose(fp);
	fileLoading = false;
	polygonSize -= 50;

}
void Mesh::dataPrinter(){
	if (vertex == NULL){
		cout << "error:: vertex not loaded" << endl;
		return;
	}
	cout << "printing vertex" << endl;

	for (int i = 0; i < vertexSize; i++){
		for (int j = 0; j < (drawMode == TRIANGLE) ? 3 : 4; j++)
			cout << vertex[j];
		cout << endl;
	}
}

string Mesh::objPolisher(string filename){

	if (filename.find("_polished") != string::npos)
		return filename;
	string prev_filename = filename;
	string new_filename = filename.insert(filename.length() - 4, "_polished");
	FILE* fp2 = fopen(new_filename.c_str(), "r");
	if (fp2 == NULL){
		FILE* fp2 = fopen(new_filename.c_str(), "w");
		FILE* fp = fopen(prev_filename.c_str(), "r");
		//file checker
		if (fp2 == NULL) {
			cout << "file open error :: " << new_filename.c_str() << endl;
			assert(false);
		}
		if (fp == NULL) {
			cout << "file open error :: " << filename.c_str() << endl;
			assert(false);
		}
		//
		char dummy;
		while (!feof(fp)){
			fscanf(fp, "%c", &dummy);
			if (dummy == '#'){//주석처리
				while (dummy != '\n'){
					fscanf(fp, "%c", &dummy);
					cout << dummy;
				}				
			}
			//cout << dummy;
			fprintf(fp2, "%c", dummy);
		}
		cout << "file created :: " << new_filename.c_str() << endl;
		return new_filename;
	}
	else{
		cout << "file created :: " << "already polished" << endl;
		return filename;
	}

}

void Mesh::setProp(int number){
	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_amb[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat mat_amb_c[] = { 0.8, 0.8, 0.2, 1.0 };
	GLfloat mat_diff[] = { 0.1, 0.5, 0.8, 1.0 };
	GLfloat mat_spec[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat no_shin[] = { 0.0 };
	GLfloat low_shin[] = { 5.0 };
	GLfloat high_shin[] = { 100.0 };
	GLfloat mat_emb[] = { 0.3, 0.2, 0.2, 0.0 };
	switch (number){
	case 0:
		glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);;
		glMaterialfv(GL_FRONT, GL_SHININESS, no_shin);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		break;
	case 1:
		glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);;
		glMaterialfv(GL_FRONT, GL_SHININESS, low_shin);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		break;
	case 2:
		glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);;
		glMaterialfv(GL_FRONT, GL_SHININESS, high_shin);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		break;
	case 3:
		glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);;
		glMaterialfv(GL_FRONT, GL_SHININESS, no_shin);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emb);
		break;
	///////////////////////////////////////////////////
	case 4:
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);;
		glMaterialfv(GL_FRONT, GL_SHININESS, no_shin);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		break;

	case 5:
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);;
		glMaterialfv(GL_FRONT, GL_SHININESS, low_shin);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		break;
	case 6:
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);;
		glMaterialfv(GL_FRONT, GL_SHININESS, high_shin);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		break;
	case 7:
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);;
		glMaterialfv(GL_FRONT, GL_SHININESS, no_shin);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emb);
		break;
	///////////////////////////////////////////////////
	case 8:
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb_c);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);;
		glMaterialfv(GL_FRONT, GL_SHININESS, no_shin);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		break;
	case 9:
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb_c);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);;
		glMaterialfv(GL_FRONT, GL_SHININESS, low_shin);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		break;
	case 10:
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb_c);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);;
		glMaterialfv(GL_FRONT, GL_SHININESS, high_shin);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		break;
	default:
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb_c);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);;
		glMaterialfv(GL_FRONT, GL_SHININESS, no_shin);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emb);
		break;
	}
}
void Mesh::changeProp(){
	this->prop = (prop + 1) % 12;
}
void Mesh::materialColor(){
	GLfloat diffMat[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat mat_spec[] = { 1, 1, 1, 1 };
	
}
void Mesh::render(){
	glPushMatrix(); {
		if (polygonSize != 0){
			//polygonSize -= 100;
			//cout << "vertex :: " << this->vertexSize << endl;
			//cout << "polygon :: " << this->polygonSize << endl;
			//cout << polygon_tri[polygonSize - 1][0] << "\t" << polygon_tri[polygonSize - 1][1] << "\t" << polygon_tri[polygonSize - 1][2] << endl;
			//cout << polygon_tri[0][0] << "\t" << polygon_tri[0][1] << "\t" << polygon_tri[0][2] << endl;
			//system("pause");
			setProp(this->prop);
			if ((fileName != "") && (fileLoading == false)){
				switch (drawMode){
				case 0: //poly
					for (int i = 0; i < polygonSize; i++){
						if (randColor){
							switch (i % 3)
							{
							case 0:
								glColor3f(1, 0, 0);
								break;
							case 1:
								glColor3f(0, 1, 0);
								break;
							default:
								glColor3f(0, 0, 1);
								break;
							}
						}
						glBegin(GL_POLYGON); {
							glNormal3f(0.0f, 0.0f, 1.0f);
							for (int j = 0; j < 3; j++){
								glVertex3f(vertex[polygon_tri[i][j] - 1][0], vertex[polygon_tri[i][j] - 1][1], vertex[polygon_tri[i][j] - 1][2]);
							}
						}
						glEnd();
					}
					break;
				case 1://wireframe
					glLineWidth(0.5);
					for (int i = 0; i < polygonSize; i++){
						glBegin(GL_LINE_LOOP); {
							//glNormal3f(0.0f, 0.0f, 1.0f);
							for (int j = 0; j < 3; j++){
								glVertex3f(vertex[polygon_tri[i][j] - 1][0], vertex[polygon_tri[i][j] - 1][1], vertex[polygon_tri[i][j] - 1][2]);
							}
						}
						glEnd();
					}
					break;
				case 2://points
					glPointSize(1);

					glBegin(GL_POINTS); {
						for (int i = 0; i < vertexSize; i++){
							glVertex3f(vertex[i][0], vertex[i][1], vertex[i][2]);
						}
					}
					glEnd();

					break;
				default:
					break;
				}
			}
		}
	}
	glPopMatrix();
}

void Mesh::modeChange(){
	this->drawMode = (drawMode + 1) % 3;
}

void Mesh::setRandColor(bool value){
	this->randColor = value;
}
int Mesh::getDrawMode(){
	return this->drawMode;
};
