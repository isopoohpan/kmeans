// kmeans.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream>
#include <string>
#pragma warning(disable : 4996)

#define MAX_DISTANCE 99999999

float** read_data(int datacount);
void write_data(float**data, int datacount);
float** kmeans(float**data, int num, int center_num, int max_iteration);
float distance(float* first, float* second, int d);

int main(int argc, char *argv[])
{
	int center_num = 10;	//	number of class
	int num = 320;		//	number of data
	float** data;		//	data
	data = read_data(num); 
	data = kmeans(data, num, center_num, 100);
	write_data(data, num);
}

float** read_data(int datacount) {

	freopen("k_means_data.txt", "r", stdin);
	float** data = new float*[datacount];
	for (int i = 0; i < datacount; i++) {
		data[i] = new float[6];
	}
	int i = 0;
	for (i = 0; i < datacount; i++) {
		scanf("%f,%f,%f,%f", &data[i][0], &data[i][1], &data[i][2], &data[i][3]);
		data[i][4] = -1; // index of class
		data[i][5] = -1; // probability 
	}
	return data;
}
void write_data(float** data, int num) {
	freopen("result.txt", "w", stdout);
	for (int i = 0; i < num; i++) {
		printf("%f,\t%f,\t%f,\t%f\t--->\tcluster%d(%f)\n", data[i][0], data[i][1], data[i][2], data[i][3], int(data[i][4]), data[i][5]);
	}
}
float distance(float* first, float* second, int d) {
	float distance = 0;
	for (int i = 0; i < d; i++) {
		distance += (first[i] - second[i]) * (first[i] - second[i]);
	}
	distance = sqrt(distance);
	return distance;
}

float** kmeans(float** data, int num, int center_num, int max_iteration) {
	float** centers = new float*[center_num];
	// init values of centers
	for (int i = 0; i < center_num; i++) {
		int r = rand() % num;
		centers[i] = new float[4];
		for (int j = 0; j < 4; j++) {
			centers[i][j] = data[r][j];
		}
		centers[i][4] = 0;
	}
	// Repeat this steps until the same points are assigned to each cluster in consecutive rounds.
	bool con = true;
	int iter = 0;

	float pre_sum_distance = MAX_DISTANCE;
	while (con && iter < max_iteration) {

		// assign points to the nearest cluster
		float sum_distance = 0;
		for (int i = 0; i < num; i++) {
			float min = MAX_DISTANCE;
			int nearest = 0;
			float sum = 0;
			for (int j = 0; j < center_num; j++) {
				float dis = distance(data[i], centers[j], 4);
				sum += dis;
				if (dis < min) {
					min = dis;
					nearest = j;
				}
			}
			data[i][4] = nearest;	// nearest cluster
			data[i][5] = (sum - min) / sum * 100;		// probabilty between data[i] and nearest th cluster
			sum_distance += min;
		}
		sum_distance /= num;
		if (pre_sum_distance > sum_distance) con = true; else con = false;
		pre_sum_distance = sum_distance;
		// determine centers again
		
		for (int i = 0; i < center_num; i++) {
			for (int j = 0; j < 5; j++) {
				centers[i][j] = 0;
			}
		}		
		for (int i = 0; i < num; i++) {
			int c = data[i][4];
			for (int j = 0; j < 4; j++) {
				centers[c][j] += data[i][j];
			}
			centers[c][4]++;
		}

		printf("======%d Iteration=========\n", iter);
		for (int i = 0; i < center_num; i++) {
			printf("--->%d---", i);
			for (int j = 0; j < 4; j++) {
				centers[i][j] /= centers[i][4];
				printf("%f,", centers[i][j]);
			}
			printf("%d\n", int(centers[i][4]));
		}
		iter++;
	}
	return data;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu