#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define D 2
#define M 5

struct point{
	float x[D];
	int group;
};

struct point centers[M];
float sum[M];

float random_(float min,float max){
	float scale = rand() / (float) RAND_MAX;
    return min + scale * ( max - min );
}

const char* getfield(char* line, struct point *p)
{
    (*p).x[0] = atof(strtok(line, " "));
		(*p).x[1] = atof(strtok(NULL, " "));
}

void init_points(struct point *p, int c){
    FILE* stream;
		if (stream = fopen("points.dat", "r")) {
			printf("true\n");
		}
		else printf("false\n");

    char line[32];
    int i;
    for (i=0; i<c; i++) {
        if (fgets(line, 32, stream)) {
            char* tmp = strdup(line);
            getfield(line, &(p[i]));
            free(tmp);
        }
    }
}

void init_centers(struct point *p){
	printf("init_centers\n");
	for (int i=0; i<M; i++){
		int randomnumber = (int)random_(0,500);
		//printf ("%d\n",randomnumber);
		centers[i]=p[randomnumber];
	}
}

void group(struct point *p){
	float dist[M];
	for (int i=0; i<M; i++){
		dist[i]=sqrt(powf(centers[i].x[0]-p->x[0],2)+powf(centers[i].x[1]-p->x[1],2));
	}
	float min = dist[0];
	int group=0;
	for (int i=0; i<M; i++){
		if (dist[i]<min){
			min=dist[i];
			group=i;
		}
	}
	p->group=group;
}

void change_centers(struct point *p){
    float sumx[M], sumy[M];
    int c[M];

    int i;
    for (i=0; i<M;i++) {
        sumx[i] = 0;
        sumy[i] = 0;
        c[i] = 0;
    }
    for (i=0;i<500;i++) {
        sumx[p[i].group] += p[i].x[0];
        sumy[p[i].group] += p[i].x[1];
        c[p[i].group] ++;
    }
    for (i=0;i<M;i++) {
        if (c[i]!=0) {
            centers[i].x[0] = sumx[i]/c[i];
            centers[i].x[1] = sumy[i]/c[i];
        }
    }
}

int main(){
	struct point p[500];
	struct point old_values[M];

	init_points(p, 500);
	int ee;
	float min=16000;

	init_centers(p);
	
	for (ee=0; ee<10; ee++){
        int e;
        while(1) {
            for (int i=0; i<500; i++){
                group(&p[i]);
            }

            for (int i=0; i<M; i++){
                old_values[i].x[0]=centers[i].x[0];
                old_values[i].x[1]=centers[i].x[1];
            }
            change_centers(p);
            int counter =0;
            for (int i=0; i<M; i++){
                if (old_values[i].x[0]==centers[i].x[0] && old_values[i].x[1]==centers[i].x[1]){
                    counter ++;
                }
            }
            if (counter == M){
                break;
            }

        }
        for (int i=0; i<5; i++) {
                printf ("Center %d: (%f, %f)\n", i, centers[i].x[0],centers[i].x[1]);
        }
        float dispersion=0;
        float dist;
        for (int i=0; i<M; i++){
            for (int j=0; j<500; j++){
                if (p[j].group==i){
                    dist = powf(centers[i].x[0]-p[j].x[0],2)+powf(centers[i].x[1]-p[j].x[1],2);
                    dispersion = dispersion + dist;
                }
            }
        }

        if (dispersion<min){
            FILE *f1 = fopen("kmeansCenters.dat", "w+");
            for(int i=0; i<M; i++){

                fprintf(f1, "%f %f\n",centers[i].x[0], centers[i].x[1]);
            }
            fclose(f1);
            min = dispersion;
        }
	}
    printf ("min dispersion %f\n",min);


	return 0;
}
