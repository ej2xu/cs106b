#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "random.h"

const double CIRCLE_SIZE = .05; // size of small circle for animation

struct Point {
    double X;
    double Y;
};

void createTriangle(Point &v1, Point &v2, Point &v3);
void iterate(Point v1, Point v2, Point v3, Point currentPoint);
Point randPoint(Point v1, Point v2, Point v3);

int main () {
	Point v1, v2, v3, currentPoint;
	InitGraphics();
	createTriangle(v1, v2, v3);
	currentPoint = randPoint(v1, v2, v3);
	iterate(v1, v2, v3, currentPoint);
	return 0;
}

void createTriangle(Point &v1, Point &v2, Point &v3) {
    int count = 0;
    while(count < 3) {
        WaitForMouseDown();
        WaitForMouseUp();
        double X = GetMouseX();
        double Y = GetMouseY();
        /* log points */
        if (count == 0) {
            MovePen(X, Y);
            v1.X = X;
            v1.Y = Y;
        } else if (count == 1) {
            v2.X = X;
            v2.Y = Y;
            DrawLine(v2.X - v1.X, v2.Y - v1.Y);
        } else {
            v3.X = X;
            v3.Y = Y;
            DrawLine(v3.X - v2.X, v3.Y - v2.Y);
            // close triangle
            DrawLine(v1.X - v3.X, v1.Y - v3.Y);
        }
        count++;
    }
}

void iterate(Point v1, Point v2, Point v3, Point currentPoint) {
	if (MouseButtonIsDown()) ExitGraphics();
	MovePen(currentPoint.X, currentPoint.Y);
	StartFilledRegion(1);
    DrawArc(CIRCLE_SIZE, 0, 360);
    EndFilledRegion();
	Point nextRand = randPoint(v1, v2, v3);
	currentPoint.X = 0.5 * (currentPoint.X + nextRand.X);
	currentPoint.Y = 0.5 * (currentPoint.Y + nextRand.Y);
	UpdateDisplay();
	iterate(v1, v2, v3, currentPoint);
}

Point randPoint(Point v1, Point v2, Point v3) {
    int rand = RandomInteger(1,3);
    if (rand == 1) return v1;
	if (rand == 2) return v2;
	else return v3;
}