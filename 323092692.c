//VS-17
#include<stdio.h>
#include<stdlib.h>
//Assigned by:
//Student's name:Or Ben-Aba
//Student's ID: 323092692
//Group number:61309-4
//lecturer:Yulia Kempner
//Totur:Arad cohen

//define struct for an interval
typedef struct Interval {
	int begin, end;
}Interval;
//define struct for the entire graph
typedef struct Graph {
	Interval*arr;
	int size;
}Graph;
//presenting our graph as an array of lists:)
typedef struct Node {
	int vertex;
	struct Node*next;
}Node;

//declarations
Graph input(int);
void printInterval(Interval);
void output(Graph);
int edges(Graph);
void multiFunction(Graph, int*, int*, int*, int*);
int*hostDegrees(Graph);
int partition(Interval*, int, int);
void quickSort(Interval[], int, int);
void swap(Interval*, Interval *);
void chromatic_number(Node**vertices, Graph g, int**colors, int*max);
Node**casting_Graph_To_ArrayOfLists(Graph);
void free_Array_Of_Lists(Node**arrlst, int rows);
int intersection(Interval, Interval);
int findMinimalColor(int*, int, Node*);
void printColors(int*, Graph, int);

void greedyColoring();

void main()
{
	greedyColoring();//calling the function, excepting to print out all the data required.
	system("PAUSE");
}
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//||||||NOTES:|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//||||||k- recives from the user the group intervals' size|||||||||||||||||||||||||||||||||||||||||||||||||||||||
//||||||maxColor-keeps the chromatic number after calculating it by using chromatic_number method||||||||||||||||||
//||||||edge-keeps the amount of edges which our intervals conceptualize|||||||||||||||||||||||||||||||||||||||||
//||||||egeCmp-keeps the number of edges which our complement graph contains|||||||||||||||||||||||||||||||||||||
//||||||maxdeg, mindeg, maxdegcmp, mindegcmp-all this are variables which "passed by reference"||||||||||||||||||
//||||||to multi function||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//||||||arr- another presentation to our intervals graph, namely a new way to present our graph,|||||||||||||||||
//||||||just by array of lists|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
void greedyColoring()
{
	int k, edge,egeCmp, maxdeg, mindeg, maxdegcmp, mindegcmp;
	Node**arr;
	int*colors, maxColor;
	system("color 0A");
	do {
		printf("Please input k\n");
		scanf_s("%d", &k);
	} while (k < 1);
	system("color 0F");
	Graph g = input(k);
	output(g);
	quickSort(g.arr, 0, g.size - 1);
	edge = edges(g);
	egeCmp = ((g.size)*(g.size - 1) / 2) - edge;
	printf("G edges = %d\n", edge);
	multiFunction(g, &maxdeg, &mindeg, &maxdegcmp, &mindegcmp);
	printf("Maximum Degree of G = %d\nMinimum Degree of G = %d\n", maxdeg, mindeg);
	arr = casting_Graph_To_ArrayOfLists(g);
	chromatic_number(arr, g,&colors,&maxColor);
	printf("Chromatic number of G = %d\n", maxColor);
	printf("G's Complement Edges = %d\n", egeCmp);
	printf("Maximum Degree of G's complement = %d\nMinimum Degree of G's complement = %d\n", maxdegcmp, mindegcmp);
	printColors(colors, g, maxColor);
	//Releasing- end of use
	free(colors);
	free_Array_Of_Lists(arr, g.size);
	free(g.arr);
}
//absorbing intervals for our graph
Graph input(int size)
{
	//Note: size of array is given
	int i;
	Graph g;
	g.arr = (Interval*)malloc(size * sizeof(Interval));//allocation memory according to user's desire
	g.size = size;
	for (i = 1; i <= size; i++)
	{
		printf("%dth Interval: ", i);
		scanf_s("%d", &g.arr[i - 1].begin);
		scanf_s("%d", &g.arr[i - 1].end);
		while (g.arr[i - 1].begin >= g.arr[i - 1].end)
		{
			printf("%dth Interval:(YOU MUST INSERT A LIGEAL INTERVAL,BEGIN<END)", i);
			scanf_s("%d", &g.arr[i - 1].begin);
			scanf_s("%d", &g.arr[i - 1].end);
		}
		printf("\n");
	}
	return g;
}

//Prints a given interval
void printInterval(Interval a)
{
	printf("[%d,%d]", a.begin, a.end);
}

//output as an intervals graph
void output(Graph g)
{
	int i;
	printf("The Intervals family is:\n");
	for (i = 0; i < g.size - 1; i++)
	{
		printInterval(g.arr[i]);
		putchar(',');
	}
	printInterval(g.arr[i]);//No comma <-> considering the given example
	putchar('\n');
}

//calculate the amount of edges
int edges(Graph g)
{
	int i, j, edges1 = 0;
	Interval temp;
	//sorting our intervals 
	quickSort(g.arr, 0, g.size - 1);
	for (i = 0; i < g.size - 1; i++)
	{
		temp = g.arr[i];
		for (j = i + 1; j < g.size; j++)
			if (temp.end > g.arr[j].begin)
				edges1++;
			else
				break;
	}
	return edges1;
}

/*This multi function calculate data of our intervals graph such as:
maximum degree
minimum degree
maximum degree of G's complement
minimum degree of G's complement
all this data passes by reference to
Note: It be given a sorted graph by its intervals*/
void multiFunction(Graph g, int*maxDeg, int*mindeg, int*maxDegCmp, int*minDegCmp)
{
	int i, *degrees = hostDegrees(g);
	*maxDeg = *mindeg = degrees[0];//initalization
	for (i = 1; i < g.size; i++)//find min and max in our array
		if (degrees[i] > *maxDeg)
			*maxDeg = degrees[i];
		else if (degrees[i] < *mindeg)
			*mindeg = degrees[i];
	free(degrees);
	*minDegCmp = g.size - (*maxDeg) - 1;
	*maxDegCmp = g.size - (*mindeg) - 1;
}

//This function returns an array for accomodating the degree of each vertex in our graph
int*hostDegrees(Graph g)
{
	int i, j, *arr = (int*)calloc(g.size, sizeof(int));
	Interval temp;
	for (i = 0; i < g.size; i++)
	{
		temp = g.arr[i];
		for (j = 0; j < i; j++)//skips the checking of i=j
			if (temp.begin > g.arr[j].end)
				arr[i]++;
			else//the array of intervals is sorted, it's useless keep checking this condition
				break;
		for (j = i + 1; j < g.size; j++)
			if (temp.end > g.arr[j].begin)
				arr[i]++;
			else//the array of intervals is sorted, it's useless keep checking this condition
				break;
	}
	return arr;
}

//calculates our chromatic graph's number
void chromatic_number(Node**vertices, Graph g, int**colors, int*max)
{
	int i;
	*max = 1;
	*colors = (int*)malloc(g.size * sizeof(int));
	for (i = 0; i < g.size; i++)//initalization of colors:
		(*colors)[i] = 1;
	for (i = 1; i < g.size; i++)
		(*colors)[i] = findMinimalColor(*colors, i, vertices[i]);
	for (i = 1; i < g.size; i++)
		if ((*colors)[i] > *max)
			*max = (*colors)[i];
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//this fuction find the minimal color of any vertex of our graph,it be done by using the following info:
/////brothers|: all neighbours of a specific vertex
///////colors|: current colors of all our vertecies
//indexAccept|: the index of the current vertex that we want to find its optimal color
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
int findMinimalColor(int*colors, int indexAccept, Node*brothers)
{
	int color = 1;
	Node*runner = brothers->next;
	while (runner&&runner->vertex < indexAccept)
	{
		if (colors[runner->vertex] == color)
		{
			color++;
			runner = brothers->next;
			continue;
		}
		runner = runner->next;
	}
	return color;
}

//prints recommanded coloring of our intervals graph
void printColors(int*colors, Graph g, int maxColor)
{
	int i, currentColor = 1, j;
	printf("Optional Coloring: ");
	for (i = 0; i < g.size; i++)
	{
		printf("{");
		for (j = 0; j < g.size; j++)//prints no comma before the first interval that suitable for him the desired color
			if (currentColor == colors[j] && maxColor >= currentColor)
			{
				printInterval(g.arr[j]);
				j++;
				break;
			}
		for (; j < g.size; j++)//prints other suitable intervals with comma before
			if (currentColor == colors[j] && maxColor >= currentColor)
			{
				putchar(',');
				printInterval(g.arr[j]);
			}
		printf("}= %d,", currentColor);
		currentColor++;
	}
	printf("\b \n");
}

//presenting our intervals graph as array of lists
Node**casting_Graph_To_ArrayOfLists(Graph g)
{
	Node**arr = (Node**)malloc(g.size * sizeof(Node*)), *runner, *inserted;
	int i, j;
	for (i = 0; i < g.size; i++)
	{
		arr[i] = (Node*)malloc(sizeof(Node));
		arr[i]->vertex = i;
		runner = arr[i];
		for (j = 0; j < g.size; j++)
		{
			if (i != j && intersection(g.arr[i], g.arr[j]))
			{
				inserted = (Node*)malloc(sizeof(Node));
				inserted->vertex = j;
				runner->next = inserted;
				runner = runner->next;
			}
		}
		runner->next = NULL;
	}
	return arr;
}

//Releasing an array of lists
void free_Array_Of_Lists(Node**arrlst, int rows)
{
	int i;
	Node*runner, *deleted;
	for (i = 0; i < rows; i++)
	{
		runner = arrlst[i];
		while (runner)
		{
			deleted = runner;
			runner = runner->next;
			free(deleted);
		}
	}
}

//returns if b intersects a
int intersection(Interval a, Interval b)
{
	return (a.begin == b.begin) || (a.begin > b.begin&&a.begin < b.end) || (b.begin >= a.begin&&b.begin < a.end);
}

int partition(Interval*arr, int low, int high)
{
	int pivot = arr[high].begin;    // pivot 
	int i = (low - 1), j;  // Index of smaller element 

	for (j = low; j <= high - 1; j++)
	{
		// If current element is smaller than the pivot 
		if (arr[j].begin < pivot)
		{
			i++;    // increment index of smaller element 
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

/* The main function that implements QuickSort
arr[] --> Array to be sorted,
low  --> Starting index,
high  --> Ending index */
void quickSort(Interval arr[], int low, int high)
{
	if (low < high)
	{
		/* pi is partitioning index, arr[p] is now
		at right place */
		int pi = partition(arr, low, high);

		// Separately sort elements before 
		// partition and after partition 
		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}
void swap(Interval *a, Interval*b)
{
	Interval temp = *a;
	*a = *b;
	*b = temp;
}