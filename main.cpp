#include <windows.h>
#include <cmath>
#include "glut.h"

// inisialisasi variabel untuk transformasi seperti translasi, rotasi atau scaling
float angle = 0.0f;					// sudut transformasi kamera
float posX = 0.0f, rotX =  0.0f;	// posisi kamera di sumbu X
float posY = 0.0f, rotY =  0.0f;	// posisi kamera di sumbu Y
float posZ = 5.0f, rotZ = -1.0f;	// posisi kamera di sumbu Z

float objectAngleX = 0.0f;			// sudut tranformasi obyek di sumbu X
float objectAngleY = 0.0f;			// sudut tranformasi obyek di sumbu Y
float objectAngleZ = 0.0f;			// sudut tranformasi obyek di sumbu Z


GLfloat controlPoints[5][3] = {
    {0.0f, 0.0f, 0.0f},   // P0
    {0.2f, 0.5f, 0.5f},   // P1
	{0.0f, 0.0f, 0.5f},     // P2
    {0.3f, 0.5f, 0.0f},    // P3
    {0.0f, 0.0f, 0.0f}     // P4
};

void drawSplineCurve(){
    int numSegments = 100;  // Number of segments for the spline curve
    // Interpolation step
    float tStep = 1.0f / numSegments;

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < numSegments; ++i)
    {
        float t = i * tStep;
        GLfloat point[3] = {0.0f, 0.0f, 0.0f};
        for (int j = 0; j < 5; ++j)
        {
            float blend = 1.0f;
            for (int k = 0; k < 5; ++k)
            {
                if (k != j)
                {
                    blend *= (t - k) / (j - k);
                }
            }
            point[0] += blend * controlPoints[j][0];
            point[1] += blend * controlPoints[j][1];
            point[2] += blend * controlPoints[j][2];
        }
        glVertex3fv(point);
    }
    glEnd();
}


// fungsi untuk menggambar kubus
void drawCube()
{
	glBegin(GL_QUADS);
	// beri warna merah di sisi depan
	glColor3f(1.0f, 0.0f, 0.0f);
	// buat sisi depan 
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f( 1.0f, -1.0f, 1.0f);
	glVertex3f( 1.0f,  1.0f, 1.0f);
	glVertex3f(-1.0f,  1.0f, 1.0f);
	// beri warna hijau di sisi belakang
	glColor3f(0.0f, 1.0f, 0.0f);
	// buat sisi belakang
	glVertex3f(-1.0f, -1.0f,-1.0f);
	glVertex3f( 1.0f, -1.0f,-1.0f);
	glVertex3f( 1.0f,  1.0f,-1.0f);
	glVertex3f(-1.0f,  1.0f,-1.0f);
	// beri warna biru di sisi kiri
	glColor3f(0.0f, 0.0f, 1.0f);
	// buat sisi kiri
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f,-1.0f);
	glVertex3f(-1.0f,  1.0f,-1.0f);
	glVertex3f(-1.0f,  1.0f, 1.0f);
	// beri warna cyan di sisi kanan
	glColor3f(0.0f, 1.0f, 1.0f);
	// buat sisi kanan
	glVertex3f( 1.0f, -1.0f, 1.0f);
	glVertex3f( 1.0f, -1.0f,-1.0f);
	glVertex3f( 1.0f,  1.0f,-1.0f);
	glVertex3f( 1.0f,  1.0f, 1.0f);
	// beri warna kuning di sisi atas
	glColor3f(1.0f, 1.0f, 0.0f);
	// buat sisi atas
	glVertex3f(-1.0f,  1.0f, 1.0f);
	glVertex3f( 1.0f,  1.0f, 1.0f);
	glVertex3f( 1.0f,  1.0f,-1.0f);
	glVertex3f(-1.0f,  1.0f,-1.0f);
	// beri warna magenta di sisi bawah
	glColor3f(1.0f, 0.0f, 1.0f);
	// buat sisi bawah
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f( 1.0f, -1.0f, 1.0f);
	glVertex3f( 1.0f, -1.0f,-1.0f);
	glVertex3f(-1.0f, -1.0f,-1.0f);
	glEnd();
}

// fungsi untuk menggambar silinder
void drawCylinder(float radius, float height, int slices, int stacks)
{
	glPushMatrix();

	GLUquadricObj* cyl = gluNewQuadric();
	gluQuadricDrawStyle(cyl, GLU_FILL);
	gluQuadricNormals(cyl, GLU_SMOOTH);
	gluQuadricOrientation(cyl, GLU_INSIDE);

	// buat tutup atas silinder
	glTranslatef(0.0f, -height/2, 0.0f);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 0.0f); // warna kuning
	gluDisk(cyl, 0.0f, radius, slices, stacks);
	// buat badan silinder
	glColor3f(1.0f, 0.0f, 0.0f); // warna merah
	gluCylinder(cyl, radius, radius, height, slices, stacks);
	// buat tutup bawah silinder
	glTranslatef(0.0f, 0.0f, height/2);
	glColor3f(1.0f, 1.0f, 0.0f); // warna kuning
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluDisk(cyl, 0.0f, radius, slices, stacks);

	glPopMatrix();
}

// fungsi untuk menggambar bola
void drawSphere(float radius, int slices, int stacks)
{
	glPushMatrix();

	glColor3f(1.0f, 0.0f, 0.0f); // warna merah
	GLUquadric *sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, radius, slices, stacks);

	glPopMatrix();
}


void drawHandphone() {
    // Draw the body of the handphone
    glColor3f(0.2f, 0.0f, 0.1f); // Blue color
    glPushMatrix();
    glScalef(1.0f, 2.0f, 0.2f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Draw the screen of the handphone
    glColor3f(0.0f, 0.0f, 0.0f); // Gray color
    glPushMatrix();
    glTranslatef(0.0f, 0.1f, 0.11f);
    glScalef(0.8f, 1.6f, 0.01f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Draw the home button
    glColor3f(0.7f, 0.7f, 0.7f); // Light gray color
    glPushMatrix();
    glTranslatef(0.0f, -0.8f, 0.11f);
    glutSolidSphere(0.1, 20, 20);
    glPopMatrix();

    // Draw the spline curve in front of the screen (without spaces)
    glPushMatrix();
    glTranslatef(0.0f, 0.1f, 0.11f); // Adjust the position of the spline curve in front of the screen
    drawSplineCurve();
    glPopMatrix();
}

// fungsi untuk menggambar obyek kubus
void drawObject()
{
	// obyek bisa dimasukkan diantara glPushMatrix() dan glPopMatrix() 
	// fungsinya agar obyek tidak terpengaruh atau mempengaruhi obyek lain
	// saat diwarnai, ditransformasi dan sebagainya
	glPushMatrix();

	// operasi transformasi rotasi obyek ke arah kanan-kiri
	glRotatef(objectAngleY, 0.0f, 1.0f, 0.0f);

	glPushMatrix();

	// operasi transformasi rotasi obyek ke arah atas-bawah
	glRotatef(objectAngleX, 1.0f, 0.0f, 0.0f);

 	
	// set warna obyek ke warna hijau (0.0f, 1.0f, 0.0f)
//	glColor3f(1.0f, 0.0f, 0.5f);

	// membuat obyek kubus
	//drawCube();
//	drawSurfaceOfRevolution(5,5);
	drawHandphone();
	glPopMatrix();

	glPopMatrix();
}

// taruh semua obyek yang akan digambar di fungsi display()
void display()
{
	// bersihkan dan reset layar dan buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);
	drawObject();
	glutSwapBuffers();
}

// inisialisasikan variabel, pencahayaan, tekstur dan pengaturan kamera pandang di fungsi init()
void init(void)
{
	// inisialisasi warna latar belakang layar dalam hal ini warna putih (1.0, 1.0, 1.0, 0.0)
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);				// mengaktifkan depth buffer
	glMatrixMode(GL_PROJECTION);		
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 1.0, 100.0);	// set proyeksi ke perspektif
	glMatrixMode(GL_MODELVIEW);				
	glLoadIdentity();						
	// inisialisasi kamera pandang
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	// enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
}

// fungsi ini digunakan bila layar akan diresize (default)
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

// fungsi untuk mengatur masukan dari keyboard 
// untuk arah kiri, kanan, atas, bawah, PgUp, dan PgDn
void keyboard(int key, int x, int y)
{
	float fraction = 0.1f;

	switch (key) 
	{
	// masukkan perintah disini bila tombol kiri ditekan
	case GLUT_KEY_LEFT:
		// dalam hal ini perintah rotasi obyek ke kiri sebanyak 1 derajat 
		objectAngleY -= 1.0f;
		glutPostRedisplay();	// update obyek
		break;
	// masukkan perintah disini bila tombol kanan ditekan
	case GLUT_KEY_RIGHT:		
		// dalam hal ini perintah rotasi obyek ke kanan sebanyak 1 derajat 
		objectAngleY += 1.0f;
		glutPostRedisplay();	// update obyek
		break;
	// masukkan perintah disini bila tombol atas ditekan
	case GLUT_KEY_UP:		
		// dalam hal ini perintah rotasi obyek ke atas sebanyak 1 derajat 
		objectAngleX -= 1.0f;
		glutPostRedisplay();	// update obyek
		break;
	// masukkan perintah disini bila tombol bawah ditekan
	case GLUT_KEY_DOWN:		
		// dalam hal ini perintah rotasi obyek ke bawah sebanyak 1 derajat 
		objectAngleX += 1.0f;
		glutPostRedisplay();	// update obyek
		break;
	// zoom in
	case GLUT_KEY_PAGE_UP:
		// masukkan perintah disini bila tombol PgUp ditekan
		posX += rotX * fraction;		
		posZ += rotZ * fraction;
		glutPostRedisplay();	// update obyek
		break;
	// zoom out
	case GLUT_KEY_PAGE_DOWN:
		// masukkan perintah disini bila tombol PgDn ditekan
		posX -= rotX * fraction;
		posZ -= rotZ * fraction;
		glutPostRedisplay();	// update obyek
		break;
	}
}

// timer untuk animasi (gunakan bila perlu)
void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(55, timer, 0);
}

// program utama
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	// set ukuran jendela tampilan
	glutInitWindowSize(480, 480);		// besarnya jendela dalam piksel dalam hal ini 300x300
	glutInitWindowPosition(100, 100);	// posisi jendela dilayar komputer dalam piksel
	// judul jendela (wajib diubah dengan informasi NAMA / NIM - JUDUL PRAKTIKUM masing-masing)
	glutCreateWindow("Maylieska D S / 2100018188 ");
	
	// panggil fungsi init untuk inisialisasi awal
	init();

	// event handler untuk display, reshape dan keyboard
	glutDisplayFunc(display);   // display
	glutReshapeFunc(reshape);   // reshape
	glutSpecialFunc(keyboard);  // keyboard
	//glutTimerFunc(0, timer, 0); // aktifkan timer bila perlu

	// looping
	glutMainLoop();

	return 0;
}
