#include "genlib.h"
#include "graphics.h"
#include "extgraph.h"
#include <iostream>

void recDrawRuler(double x, double y, double w, double h) {
	if (h < 0.001) return;
	MovePen(x + w / 2, y);
	DrawLine(0, h);
	recDrawRuler(x, y, w / 2, h / 2);
	recDrawRuler(x + w / 2, y, w / 2, h / 2);
}

void DrawRuler(double x, double y, double w, double h) {
	MovePen(x, y);
	DrawLine(w, 0);
	recDrawRuler(x, y, w, h);
}

int main() {
	 InitGraphics();
    double x,y = 0;
    double width = GetWindowWidth();
    double height = GetWindowHeight();
    DrawRuler(x,y,width,height);
	return 0;
}