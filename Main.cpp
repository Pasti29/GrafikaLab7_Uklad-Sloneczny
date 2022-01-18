#define _USE_MATH_DEFINES

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>
#include <string>
#include "Planet.h"
#include "Textures.h"
#include <cmath>

static float pix2angle;     // przelicznik pikseli na stopnie

static int status = 0;
// stan klawiszy myszy
// 0 - nie naciśnięto żadnego klawisza
// 1 - naciśnięty zostać lewy klawisz
// 2 - naciśnięty prawy klawisz

static int x_pos_old = 0; // poprzednia pozycja kursora myszy
static int delta_x = 0; // różnica pomiędzy pozycją bieżącą i poprzednią kursora myszy

static int y_pos_old = 0; // poprzednia pozycja kursora myszy
static int delta_y = 0; // różnica pomiędzy pozycją bieżącą i poprzednią kursora myszy

static float viewer[] = { 158'932, 94'023.4, 302'876 };
static float lookAtPoint[] = { 158'930, 94'022.4, 302'866 };
static float azymuth = -1.7892;
static float elevation = -0.108;

double day = 0;

int distanceFromCenter[] = { 57'900, 108'200, 149'600, 227'900, 778'600, 1'433'500, 2'872'500, 4'498'100 };
double radius[] = { 2'439.5, 6'052.0, 6'378.0, 3'396.0, 71'492.0, 60'268.0, 25'559.0, 24'794.0, 34'825.0 };
int daysInYear[] = { 88, 224, 365, 686, 4'333, 10'756, 30'707, 60'223 };



Planet sun(radius[8], "Tekstury\\sun.tga");
Planet planets[] = {
	Planet(radius[0], "Tekstury\\mercury.tga", distanceFromCenter[0], daysInYear[0]),
	Planet(radius[1], "Tekstury\\venus.tga", distanceFromCenter[1], daysInYear[1]),
	Planet(radius[2], "Tekstury\\earth.tga", distanceFromCenter[2], daysInYear[2]),
	Planet(radius[3], "Tekstury\\mars.tga", distanceFromCenter[3], daysInYear[3]),
	Planet(radius[4], "Tekstury\\jupiter.tga", distanceFromCenter[4], daysInYear[4]),
	Planet(radius[5], "Tekstury\\saturn.tga", distanceFromCenter[5], daysInYear[5]),
	Planet(radius[6], "Tekstury\\uranus.tga", distanceFromCenter[6], daysInYear[6]),
	Planet(radius[7], "Tekstury\\neptune.tga", distanceFromCenter[7], daysInYear[7])
};

int segments = 1000;

void zoom(bool zoomed) {
	float x = 200 * (lookAtPoint[0] - viewer[0]);
	float y = 200 * (lookAtPoint[1] - viewer[1]);
	float z = 200 * (lookAtPoint[2] - viewer[2]);
	if (zoomed) {
		x *= -1;
		y *= -1;
		z *= -1;
	}
	viewer[0] += x;
	viewer[1] += y;
	viewer[2] += z;
	lookAtPoint[0] += x;
	lookAtPoint[1] += y;
	lookAtPoint[2] += z;
}

void drawOrbit(Planet planet) {
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.6, 0.6, 0.6);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++) {
		float angle = 1.0 * float(i) / float(360);
		float x = planet.getDistanceFromCenter() * cos(2 * M_PI * angle);
		float y = planet.getDistanceFromCenter() * sin(2 * M_PI * angle);

		glVertex3f(x, 0, y);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0, 1.0, 1.0);
}

void solarSystem() {
	sun.showTexture();
	GLUquadricObj* quadricObj = gluNewQuadric();
	gluQuadricDrawStyle(quadricObj, GLU_FILL);
	gluQuadricNormals(quadricObj, GLU_SMOOTH);
	gluQuadricTexture(quadricObj, GL_TRUE);
	gluSphere(quadricObj, sun.getRadius(), 20, 20);
	

	for (Planet planet : planets) {
		drawOrbit(planet);
		planet.setAngle(day);
		planet.setX();
		planet.setZ();
		glTranslatef(planet.getX(), 0, planet.getZ());
		planet.showTexture();
		gluSphere(quadricObj, planet.getRadius(), 20, 20);
		glTranslatef(-planet.getX(), 0, -planet.getZ());
	}
}

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(viewer[0], viewer[1], viewer[2], lookAtPoint[0], lookAtPoint[1], lookAtPoint[2], 0.0, 1.0, 0.0);

	if (status == 2) {
		if (delta_y != 0) {
			zoom(delta_y > 0);
		}
	}else if (status == 1) {
		azymuth += delta_x * pix2angle * 0.01;
		elevation -= delta_y * pix2angle * 0.01;
		if (sin(elevation) >= 0.99) {
			elevation = 1.44;
		}
		if (sin(elevation) <= -0.99) {
			elevation = -1.44;
		}
	}
	lookAtPoint[0] = 10 * cos(azymuth) * cos(elevation) + viewer[0];
	lookAtPoint[1] = 10 * sin(elevation) + viewer[1];
	lookAtPoint[2] = 10 * sin(azymuth) * cos(elevation) + viewer[2];

	solarSystem();

	glutSwapBuffers();
}

void changeDay() {
	day += 0.1;
	Sleep(1);
	renderScene();

	if (day > 60223) {
		day -= 60223;
	}

}

// Funkcja wyświetlająca w konsoli informacje o opcjach programu
void initProgram() {
	std::cout << "Program z laboratorium 7 - projekt ukladu slonecznego\n";

}

// Funkcja obsługuje wciśnięcie przycisku na myszce
void mouse(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		y_pos_old = y;
		status = 1;
	} else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		y_pos_old = y;
		//y_pos_old *= 100;
		status = 2;
	} else {
		status = 0;          // nie został wciśnięty żaden klawisz
	}

	if ((btn == GLUT_LEFT_BUTTON && state == GLUT_UP) || (btn == GLUT_RIGHT_BUTTON && state == GLUT_UP)) {
		delta_x = 0;
		delta_y = 0;
	}

	renderScene();
}

// Funkcja obsługująca działanie programu za pomocą klawiszy klawiatury
void keys(unsigned char key, int x, int y) {
	if (key == 'r') {
		viewer[0] = 158'932;
		viewer[1] = 94'023.4;
		viewer[2] = 302'876;
		lookAtPoint[0] = 158'930;
		lookAtPoint[1] = 94'022.4;
		lookAtPoint[2] = 302'866;
		azymuth = -1.7892;
		elevation = -0.108;
	}

	if (key == (char)27) exit(0);

	renderScene(); // przerysowanie obrazu sceny
}

// Funkcja "monitoruje" położenie kursora myszy i ustawia wartości odpowiednich
// zmiennych globalnych
void motion(GLsizei x, GLsizei y) {

	delta_x = x - x_pos_old;     // obliczenie różnicy położenia kursora myszy
	delta_y = y - y_pos_old;

	x_pos_old = x;            // podstawienie bieżącego położenia jako poprzednie
	y_pos_old = y;

	glutPostRedisplay();     // przerysowanie obrazu sceny
}

// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokość i szerokość okna) są
// przekazywane do funkcji za każdym razem, gdy zmieni się rozmiar okna.
void changeSize(GLsizei horizontal, GLsizei vertical) {

	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie

	glMatrixMode(GL_PROJECTION);
	// Przełączenie macierzy bieżącej na macierz projekcji

	glLoadIdentity();
	// Czyszczenie macierzy bieżącej

	gluPerspective(70, 1.6, 1.0, 10'000'000.0);

	if (horizontal <= 1.6 * vertical)
		glViewport(0, (vertical - horizontal / 1.6) / 2, 1.6 * vertical, vertical);

	else
		glViewport((horizontal - 1.6 * vertical) / 2, 0, 1.6 * vertical, vertical);
	// Ustawienie wielkości okna widoku (viewport) w zależności
	// relacji pomiędzy wysokością i szerokością okna

	glMatrixMode(GL_MODELVIEW);
	// Przełączenie macierzy bieżącej na macierz widoku modelu

	glLoadIdentity();
	// Czyszczenie macierzy bieżącej

}

void myInit() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//          --<<OŚWIETLENIE>>--

	/*************************************************************************************/
// Definicja materiału, z jakiego zrobiony jest obiekt

	GLfloat mat_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	// współczynniki ka =[kar,kag,kab] dla światła otoczenia

	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// współczynniki kd =[kdr,kdg,kdb] światła rozproszonego

	GLfloat mat_specular[] = { 0.1, 0.1, 0.0, 0.1 };
	// współczynniki ks =[ksr,ksg,ksb] dla światła odbitego

	GLfloat mat_shininess = { 20.0 };
	// współczynnik n opisujący połysk powierzchni

	GLfloat mat_emission[] = { 0.4, 0.4, 0.4, 1.0 };

/*************************************************************************************/
// Definicja stałych odpowiadających za osłabienie światła dla obu źródeł

	GLfloat att_constant = { 1.0 };
	// składowa stała ds dla modelu zmian oświetlenia w funkcji
	// odległości od źródła

	GLfloat att_linear = { 0.05 };
	// składowa liniowa dl dla modelu zmian oświetlenia w funkcji
	// odległości od źródła

	GLfloat att_quadratic = { 0.001 };
	// składowa kwadratowa dq dla modelu zmian oświetlenia w funkcji
	// odległości od źródła

/*************************************************************************************/
// Definicja pierwszego źródła światła

	GLfloat lightPosition[] = { 0.0, 0.0, 0.0 };

	GLfloat light_ambient[] = { 0.1, 0.1, 0.05, 1.0 };
	// składowe intensywności świecenia źródła światła otoczenia
	// Ia = [Iar,Iag,Iab]

	//GLfloat light_emission[] = { 1.0, 1.0, 0.8, 1.0 };

	GLfloat light_diffuse[] = { 1.0, 1.0, 0.8, 1.0 };
	// składowe intensywności świecenia źródła światła powodującego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// składowe intensywności świecenia źródła światła powodującego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

/*************************************************************************************/
// Ustawienie parametrów materiału


	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);


	/*************************************************************************************/
	// Ustawienie parametrów pierwszego źródła światła

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	/*************************************************************************************/
	// Ustawienie opcji systemu oświetlania sceny

	glShadeModel(GL_SMOOTH); // włączenie łagodnego cieniowania
	glEnable(GL_LIGHTING);   // włączenie systemu oświetlenia sceny
	glEnable(GL_LIGHT0);     // włączenie źródła o numerze 0
	glEnable(GL_DEPTH_TEST); // włączenie mechanizmu z-bufora

	

	//          --<<TEKSTUROWANIE>>--

	// Teksturowanie będzie prowadzone tylko po jednej stronie ściany
	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);

	// Włączenie mechanizmu teksturowania
	glEnable(GL_TEXTURE_2D);

	// Ustalenie trybu teksturowania
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Określenie sposobu nakładania tekstur
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//renderScene();
}

int main() {
	initProgram();

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(1000, 1000);

	glutCreateWindow("Układ słoneczny");

	glutDisplayFunc(renderScene);

	glutReshapeFunc(changeSize);

	myInit();

	glutKeyboardFunc(keys);

	glutMouseFunc(mouse);

	glutMotionFunc(motion);

	glEnable(GL_DEPTH_TEST); // włączenie mechanizmu z-bufora

	glutIdleFunc(changeDay);

	glutMainLoop();
}
