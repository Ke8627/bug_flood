//
// Created by nishant on 15/5/16.
//

#include <bug_flood/helper_functions.h>

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim)
{
std::vector<std::string> elems;
split(s, delim, elems);
return elems;
}

bool IsIntersecting(Line first, Line second, Point &intersection, double &distance)
{

	//defining start points
	double p0_x = first.start.x;
	double p0_y = first.start.y;
	double p1_x = first.end.x;
	double p1_y = first.end.y;

	//defining end points
	double p2_x = second.start.x;
	double p2_y = second.start.y;
	double p3_x = second.end.x;
	double p3_y = second.end.y;

	//resetting intersection to a predefined null value
	intersection.x = INT_MAX;
	intersection.y = INT_MAX;
	intersection.z = 0;

	double s1_x, s1_y, s2_x, s2_y;
	s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
	s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

	double s, t;
	s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

	double i_x,i_y;

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		i_x = p0_x + (t * s1_x);
		i_y = p0_y + (t * s1_y);

		intersection.x = i_x;
		intersection.y = i_y;


		//set distance from start of first line to intersection
		distance = getEuclideanDistance(first.start, intersection);
		return true;
	}

	return false; // No collision
}

double getEuclideanDistance(Point first, Point second)
{
	return sqrt(pow(first.x - second.x, 2) + pow(first.y - second.y, 2));
}

double getDirectionAngleRadian(Point goal, Point source)
{
	return atan2(goal.y - source.y, goal.x - source.x);
}

double getDirectionAngleDegrees(Point goal, Point source)
{
	double rad = atan2(goal.y - source.y, goal.x - source.x);
	return (rad * 57.2957795131); //pre calculated 180/pi
}

void prunePath(vector<Point> &inPath, Environment & environment, double &cost)
{
	vector< Point > prunedPath;
	double prunedCost = 0;

	//entering first element before pruning starts
	prunedPath.push_back(inPath[0]);

	bool intersectionResult = false;
	int i=0;
	int j;
	int lastIntersection = 1;

	Point intersection;
	double tempCost;
	int tempID;
	while (i < inPath.size()-1)
	{
		lastIntersection = i+1;
		for(j=i+2;j<inPath.size();j++)
		{

			intersectionResult = environment.getObstacleIntersection(prunedPath[prunedPath.size() - 1], inPath[j], intersection, tempCost, tempID);
			if(!intersectionResult) //intersection not detected
			{
				lastIntersection = j;
			}
		}
		i = lastIntersection;
		prunedPath.push_back(inPath[i]);
		prunedCost += sqrt(pow(prunedPath[prunedPath.size() - 1].x - prunedPath[prunedPath.size() - 2].x,2)+pow(prunedPath[prunedPath.size() - 1].y - prunedPath[prunedPath.size() - 2].y,2));
	}
	//safety check
	if(i == (inPath.size()-2))
	{
		prunedPath.push_back(inPath[inPath.size() - 1]);
		prunedCost += sqrt(pow(prunedPath[prunedPath.size() - 1].x - prunedPath[prunedPath.size() - 2].x, 2) +
								  pow(prunedPath[prunedPath.size() - 1].y - prunedPath[prunedPath.size() - 2].y, 2));
	}

	inPath = prunedPath;
	cost = prunedCost;
}


