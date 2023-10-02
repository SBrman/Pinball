#pragma once
#include "Vec.hpp"
#include <vector>
#include <cmath>


std::vector<Vec> rotate_box(double x, double y, double width, double height, double angle) {
	
	Vec center{x+width/2, y+height/2};

	Vec right{ std::cos(angle), std::sin(angle) };
	right *= (width/2);

	Vec up{ -sin(angle), cos(angle) };
	up *= (-height/2);
	
	Vec pos{ x, y };
	Vec p1 = center + right + up;
	Vec p2 = center + right - up;
	Vec p3 = center - right - up;
	Vec p4 = center - right + up;

	return std::vector<Vec> {p1, p2, p3, p4};
}


std::vector<Vec> rotated_polygon(std::vector<Vec>& points, int n, Vec pivot, float angle) {
	
	std::vector<Vec> rotated_points;
	
	for (int i = 0; i < n; i++) {
		// Vector of point from pivot
		Vec vpp = points[i] - pivot;
		// Rotate new point
		Vec rotated_vpp{
			vpp.x * std::cos(angle) - vpp.y * std::sin(angle),
			vpp.x * std::sin(angle) + vpp.y * std::cos(angle),
		};
		// Translate
		Vec rotated_point = vpp + pivot;
		rotated_points.push_back(rotated_point);
	}
	return rotated_points;
}

Vec rotate_point(Vec point, Vec pivot, float angle) {
	
	// Vector of point from pivot
	Vec vpp = point - pivot;

	// Rotate new point
	Vec rotated_vpp{
		vpp.x * std::cos(angle) - vpp.y * std::sin(angle),
		vpp.x * std::sin(angle) + vpp.y * std::cos(angle),
	};

	// Translate
	Vec rotated_point = vpp + pivot;

	return rotated_point;
}



std::vector<Vec> translate(const std::vector<Vec>& points, Vec translater) {
	std::vector<Vec> newPoints;
	for (Vec point : points) {
		newPoints.push_back(translater + point);
	}
	return newPoints;
}
