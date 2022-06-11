#include <GL/glut.h>
#include <cstdlib>
#include <iostream>

static GLfloat xRang = 2.0, yRang = 2.0, zRang = 2.0;
GLuint textures[7];
int theta = 0.0;
enum class Move {
    NONE,
    FRONT,
    RIGHT,
    LEFT,
    BOTTOM,
    TOP,
    BACK,
    WHOLEX,
    WHOLEY,
    WHOLEZ
};
Move move = Move::NONE;
float sense = 1.0;

int nMov = -1;
int cube[55] = {
	0,2,0,2,0,2,0,2,0, // Izquierda
	1,3,1,3,1,3,1,3,1, // Frente
	2,0,2,0,2,0,2,0,2, // Derecha
	3,1,3,1,3,1,3,1,3, // Atras
	4,5,4,5,4,5,4,5,4, // Arriba
	5,4,5,4,5,4,5,4,5, // Abajo
	6, // Valor extra
};

int indexes[2][54] = {
        {0,1,45,3,4,46,6,7,47,15,12,9,16,13,10,17,14,11,42,19,20,43,22,23,44,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,8,5,2,24,21,18,48,49,50,51,52,53},
        {0,1,44,3,4,43,6,7,42,11,14,17,10,13,16,9,12,15,47,19,20,46,22,23,45,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,18,21,24,2,5,8,48,49,50,51,52,53}
};

int vertices[56][3] = {
	{-3, -3,  3},		// 0
	{-1, -3,  3},		// 1
	{ 1, -3,  3},		// 2
	{ 3, -3,  3},       // 3
	{ 3, -1,  3},       // 4
	{ 3,  1,  3},       // 5
	{ 3,  3,  3},       // 6
	{ 1,  3,  3},       // 7
	{-1,  3,  3},       // 8
	{-3,  3,  3},       // 9
	{-3,  1,  3},       // 10
	{-3, -1,  3},       // 11

	{-3, -3,  1},		// 12
	{-1, -3,  1},		// 13
	{ 1, -3,  1},		// 14
	{ 3, -3,  1},       // 15
	{ 3, -1,  1},       // 16
	{ 3,  1,  1},       // 17
	{ 3,  3,  1},       // 18
	{ 1,  3,  1},       // 19
	{-1,  3,  1},       // 20
	{-3,  3,  1},       // 21
	{-3,  1,  1},       // 22
	{-3, -1,  1},       // 23

	{-3, -3, -1},		// 24
	{-1, -3, -1},		// 25
	{ 1, -3, -1},		// 26
	{ 3, -3, -1},       // 27
	{ 3, -1, -1},       // 28
	{ 3,  1, -1},       // 29
	{ 3,  3, -1},       // 30
	{ 1,  3, -1},       // 31
	{-1,  3, -1},       // 32
	{-3,  3, -1},       // 33
	{-3,  1, -1},       // 34
	{-3, -1, -1},       // 35

	{-3, -3, -3},		// 36
	{-1, -3, -3},		// 37
	{ 1, -3, -3},		// 38
	{ 3, -3, -3},       // 39
	{ 3, -1, -3},       // 40
	{ 3,  1, -3},       // 41
	{ 3,  3, -3},       // 42
	{ 1,  3, -3},       // 43
	{-1,  3, -3},       // 44
	{-3,  3, -3},       // 45
	{-3,  1, -3},       // 46
	{-3, -1, -3},       // 47

	{-1, -1, 3},       // 48
	{-1,  1, 3},       // 49
	{ 1, -1, 3},       // 50
	{ 1,  1, 3},       // 51

	{ 1, -1, -3},       // 52
	{ 1,  1, -3},       // 53
	{-1, -1, -3},       // 54
	{-1,  1, -3}        // 55
};

void MoveColors(int n){
    int copia[54];
    int i;

    for(i=0; i<54; i++){
        copia[i] = cube[indexes[n][i]];
    }

    for(i=0; i<54; i++){
        cube[i] = copia[i];
    }
}

GLuint LoadTexture(const char* filename)
{
	GLuint texture;
	int width, height;
	unsigned char* data;
	FILE* file;

	file = fopen(filename, "rb");
	if (file == nullptr)
		return 0;

	width = 256;
	height = 256;

	data = (unsigned char*)malloc(width * height * 3);
	fseek(file, 54, 0);

	fread(data, width * height * 3, 1, file);
	fclose(file);

	for (int i = 0; i < width * height; i++)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	free(data);
	return texture;
}



static void resize(int width, int height)
{
    glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-xRang, xRang, -yRang, yRang, -zRang, zRang);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void sticker(int n, int v1, int v2, int v3, int v4)
{
    double len = 0.35;
    int color = cube[n];
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[color]);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0);
        glVertex3d(vertices[v1][0] * len, vertices[v1][1] * len, vertices[v1][2] * len);
        glTexCoord2f(0.0, 0.0);
        glVertex3d(vertices[v2][0] * len, vertices[v2][1] * len, vertices[v2][2] * len);
        glTexCoord2f(1.0, 0.0);
        glVertex3d(vertices[v3][0] * len, vertices[v3][1] * len, vertices[v3][2] * len);
        glTexCoord2f(1.0, 1.0);
        glVertex3d(vertices[v4][0] * len, vertices[v4][1] * len, vertices[v4][2] * len);
	glEnd();
	glDisable(GL_TEXTURE);
}

static void esquina1(){
	sticker(9, 3, 4, 16, 15);
    sticker(42, 2, 50, 4, 3);
	sticker(2, 2, 3, 15, 14);
}

static void esquina2(){
	sticker(11, 5, 6, 18, 17);
    sticker(18, 6, 7, 19, 18);
	sticker(44, 51, 7, 6, 5);
}

static void esquina3(){
	sticker(20, 8, 9, 21, 20);
    sticker(27, 9, 10, 22, 21);
	sticker(38, 10, 9, 8, 49);
}

static void esquina4(){
	sticker(0, 0, 1, 13, 12);
    sticker(29, 11, 0, 12, 23);
	sticker(36, 0, 11, 48, 1);
}

void esquina5() {
	sticker(8, 26, 27, 39, 38);
	sticker(15, 27, 28, 40, 39);
	sticker(45, 39, 40, 52, 38);
}

void esquina6() {
	sticker(17, 29, 30, 42, 41);
	sticker(24, 30, 31, 43, 42);
	sticker(47, 41, 42, 43, 53);
}

void esquina7() {
	sticker(26, 32, 33, 45, 44);
	sticker(33, 33, 34, 46, 45);
	sticker(53, 55, 44, 45, 46);
}

void esquina8() {
	sticker(6, 24, 25, 37, 36);
	sticker(35, 35, 24, 36, 47);
	sticker(51, 37, 54, 47, 36);
}

void arista1() {
	sticker(1, 1, 2, 14, 13);
	sticker(39, 1, 48, 50, 2);
}

void arista2() {
	sticker(10, 4, 5, 17, 16);
	sticker(43, 50, 51, 5, 4);
}

void arista3() {
	sticker(19, 7, 8, 20, 19);
	sticker(41, 49, 8, 7, 51);
}

void arista4() {
	sticker(28, 10, 11, 23, 22);
	sticker(37, 11, 10, 49, 48);
}

void arista5() {
	sticker(5, 14, 15, 27, 26);
	sticker(12, 15, 16, 28, 27);
}

void arista6() {
	sticker(14, 17, 18, 30, 29);
	sticker(21, 18, 19, 31, 30);
}

void arista7() {
	sticker(23, 20, 21, 33, 32);
	sticker(30, 21, 22, 34, 33);
}

void arista8() {
	sticker(3, 12, 13, 25, 24);
	sticker(32, 23, 12, 24, 35);
}

void arista9() {
	sticker(7, 25, 26, 38, 37);
	sticker(48, 38, 52, 54, 37);
}

void arista10() {
	sticker(16, 28, 29, 41, 40);
	sticker(46, 40, 41, 53, 52);
}

void arista11() {
	sticker(25, 31, 32, 44, 43);
	sticker(50, 53, 43, 44, 55);
}

void arista12() {
	sticker(34, 34, 35, 47, 46);
	sticker(52, 54, 55, 46, 47);
}

void drawCube(){
    esquina1();
    esquina2();
    esquina3();
    esquina4();
    esquina5();
    esquina6();
    esquina7();
    esquina8();

    arista1();
    arista2();
    arista3();
    arista4();

    arista5();
    arista6();
    arista7();
    arista8();

    arista9();
    arista10();
    arista11();
    arista12();

    sticker(4, 13, 14, 26, 25);         // Centro izquierdo
    sticker(13, 16, 17, 29, 28);        // Centro frontal
    sticker(22, 19, 20, 32, 31);        // Centro derecho
    sticker(31, 22, 23, 35, 34);        // Centro posterior
    sticker(40, 48, 49, 51, 50);        // Centro superior
    sticker(49, 52, 53, 55, 54);        // Centro inferior
}

void GiroX(){
    glRotatef(theta * sense, 1.0f, 0.0f, 0.0f);
    esquina1();
    esquina2();
    esquina3();
    esquina4();
    esquina5();
    esquina6();
    esquina7();
    esquina8();

    arista1();
    arista2();
    arista3();
    arista4();

    arista5();
    arista6();
    arista7();
    arista8();

    arista9();
    arista10();
    arista11();
    arista12();

    sticker(4, 13, 14, 26, 25);         // Centro izquierdo
    sticker(13, 16, 17, 29, 28);        // Centro frontal
    sticker(22, 19, 20, 32, 31);        // Centro derecho
    sticker(31, 22, 23, 35, 34);        // Centro posterior
    sticker(40, 48, 49, 51, 50);        // Centro superior
    sticker(49, 52, 53, 55, 54);        // Centro inferior
}

void GiroY(){
    glRotatef(theta * sense, 0.0f, 1.0f, 0.0f);
    esquina1();
    esquina2();
    esquina3();
    esquina4();
    esquina5();
    esquina6();
    esquina7();
    esquina8();

    arista1();
    arista2();
    arista3();
    arista4();

    arista5();
    arista6();
    arista7();
    arista8();

    arista9();
    arista10();
    arista11();
    arista12();

    sticker(4, 13, 14, 26, 25);         // Centro izquierdo
    sticker(13, 16, 17, 29, 28);        // Centro frontal
    sticker(22, 19, 20, 32, 31);        // Centro derecho
    sticker(31, 22, 23, 35, 34);        // Centro posterior
    sticker(40, 48, 49, 51, 50);        // Centro superior
    sticker(49, 52, 53, 55, 54);        // Centro inferior
}

void giroFrontal(){
    esquina3();
    esquina4();
    esquina7();
    esquina8();

    arista1();
    arista3();
    arista4();

    arista7();
    arista8();

    arista9();
    arista11();
    arista12();

    sticker(4, 13, 14, 26, 25);         // Centro izquierdo
    sticker(22, 19, 20, 32, 31);        // Centro derecho
    sticker(31, 22, 23, 35, 34);        // Centro posterior
    sticker(40, 48, 49, 51, 50);        // Centro superior
    sticker(49, 52, 53, 55, 54);        // Centro inferior
    sticker(54, 2, 7, 43, 38);

    glRotatef(theta * sense, 1.0f, 0.0f, 0.0f);
    esquina1();
    esquina2();
    esquina5();
    esquina6();
    arista2();
    arista5();
    arista6();
    arista10();
    sticker(13, 16, 17, 29, 28);        // Centro frontal
}


void giroDerecho(){
    esquina1();
    esquina4();
    esquina5();
    esquina8();

    arista1();
    arista2();

    arista4();

    arista5();
    arista8();

    arista9();
    arista10();

    arista12();

    sticker(4, 13, 14, 26, 25);         // Centro izquierdo
    sticker(13, 16, 17, 29, 28);        // Centro frontal

    sticker(31, 22, 23, 35, 34);        // Centro posterior
    sticker(40, 48, 49, 51, 50);        // Centro superior
    sticker(49, 52, 53, 55, 54);        // Centro inferior

    sticker(54, 5, 10, 46, 41);

    glRotatef(theta * sense, 0.0f, 1.0f, 0.0f);
    esquina2();
    esquina3();
    esquina6();
    esquina7();
    arista3();
    arista6();
    arista7();
    arista11();
    sticker(22, 19, 20, 32, 31);        // Centro derecho


}


static void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

    GLfloat rotate_x = -54.75;
    GLfloat rotate_y = 0;
    GLfloat rotate_z = -135;

    glRotatef(rotate_x, 1.0f, 0.0f, 0.0f);
	//glRotatef(rotate_y, 0.0f, 1.0f, 0.0f);
	glRotatef(rotate_z, 0.0f, 0.0f, 1.0f);


	switch(move){
        case Move::NONE:
            drawCube();
            break;
        case Move::RIGHT:
            giroDerecho();
            break;
        case Move::FRONT:
            giroFrontal();
            break;
        case Move::WHOLEX:
            GiroX();
            break;
        case Move::WHOLEY:
            GiroY();
            break;
        case Move::LEFT:
            break;
        case Move::BOTTOM:
            break;
        case Move::TOP:
            break;
        case Move::BACK:
            break;
        case Move::WHOLEZ:
            break;
    }
	//drawCube();
    //giroDerecho();
	//giroFrontal();


	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch(key){
        case 'R':
            move = Move::RIGHT;
            sense = -1.0;
            nMov = -1;
            break;
        case 'r':
            move = Move::RIGHT;
            sense = 1.0;
            nMov = -1;
            break;
        case 'F':
            move = Move::FRONT;
            sense = -1.0;
            nMov = 0;
            break;
        case 'f':
            move = Move::FRONT;
            sense = 1.0;
            nMov = 1;
            break;

        case 'X':
            move = Move::WHOLEX;
            sense = -1.0;
            break;
        case 'x':
            move = Move::WHOLEX;
            sense = 1.0;
            break;
        case 'Y':
            move = Move::WHOLEY;
            sense = -1.0;
            break;
        case 'y':
            move = Move::WHOLEY;
            sense = 1.0;
            break;
        case 'Z':
            move = Move::WHOLEZ;
            sense = -1.0;
            break;
        case 'z':
            move = Move::WHOLEZ;
            sense = 1.0;
            break;
    }

    glutPostRedisplay();
}

static void idle()
{
    if(move != Move::NONE)
        theta++;

    if(theta > 90) {
        move = Move::NONE;
        if(nMov > -1)
            MoveColors(nMov);
    }
    glutPostRedisplay();
}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);										// Innicializar glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);	// Modo de visualizaciòn
	glutInitWindowSize(700, 700);								// Inicializar ventana
	glutInitWindowPosition(0, 0);								//
	glutCreateWindow("Rubik OpenGL");							//



    textures[0] = LoadTexture("/home/fwcheesecake/CLionProjects/opengl-rubik/img/stckNaranja.bmp");
    textures[1] = LoadTexture("/home/fwcheesecake/CLionProjects/opengl-rubik/img/stckBlanco.bmp");
    textures[2] = LoadTexture("/home/fwcheesecake/CLionProjects/opengl-rubik/img/stckRojo.bmp");
    textures[3] = LoadTexture("/home/fwcheesecake/CLionProjects/opengl-rubik/img/stckAmarillo.bmp");
    textures[4] = LoadTexture("/home/fwcheesecake/CLionProjects/opengl-rubik/img/stckAzul.bmp");
    textures[5] = LoadTexture("/home/fwcheesecake/CLionProjects/opengl-rubik/img/stckVerde.bmp");
    //textures[6] = LoadTexture("/home/fwcheesecake/CLionProjects/opengl-rubik/img/stcPlano.bmp");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glutReshapeFunc(resize);									// Función que GLUT utilizará cuando redimensiona la ventana
	glutDisplayFunc(display);									// Funciòn que GLUT invoca cada que redibuja
	glutKeyboardFunc(key);
	glutIdleFunc(idle);
	glutMainLoop();												// Todo funciona en un ciclo
    return EXIT_SUCCESS;
}
