#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>
#include <string>
#include "Planet.h"
#include "Textures.h"

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

static float viewer[] = { 0, 200, 1000 };
static float lookAtPoint[] = { 0, 0, 0 };
static float azymuth = 0;
static float elevation = 0;
static float azymuth2 = 0;
static float elevation2 = 0;

int day = 0;

int distanceFromCenter[] = { 57, 108, 149, 227, 778, 1433, 2872, 4498 };
double radius[] = { 4.87, 12.1, 12.76, 6.79, 71, 60, 25, 24, 30 };
int daysInYear[] = { 88, 224, 365, 686, 4333, 10756, 30707, 60223 };



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
	float x = (lookAtPoint[0] - viewer[0]);
	float y = (lookAtPoint[1] - viewer[1]);
	float z = (lookAtPoint[2] - viewer[2]);
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

void solarSystem() {
	sun.showTexture();
	glutSolidSphere(sun.getRadius(), segments, segments);

	for (Planet planet : planets) {
		planet.setAngle(day);
		planet.setX();
		planet.setZ();
		glTranslatef(planet.getX(), 0, planet.getZ());
		planet.showTexture();
		glutSolidSphere(planet.getRadius(), segments, segments);
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
	day++;
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
		status = 1;          // wciśnięty został lewy klawisz myszy
	} else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		y_pos_old = y;
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
		viewer[0] = 0;
		viewer[1] = 500;
		viewer[2] = 0;
		lookAtPoint[0] = 0;
		lookAtPoint[1] = 0;
		lookAtPoint[2] = 0;
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

	gluPerspective(70, 1.6, 1.0, 100000.0);

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

	//          --<<TEKSTUROWANIE>>--

	// Teksturowanie będzie prowadzone tylko po jednej stronie ściany
	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);

	// Włączenie mechanizmu teksturowania
	glEnable(GL_TEXTURE_2D);

	// Ustalenie trybu teksturowania
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Określenie sposobu nakładania tekstur
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
