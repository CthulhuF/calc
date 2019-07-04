#include <stdio.h>
#include <stdlib.h>

/*
 * 1-st sign - sign of operation
 * 2-nd sign - v/s vector or scalar calculator
 * 3-rd sign - number of coordinates
 * 4-th and next - numerical value of coordinates
 * Single commands separated by spaces or line breaks
 */



typedef struct linked_list{
    char command /* + - * */, operation /* v/s */;
    int size; /*length of vectors*/
    float *first /*first number or first vector*/, *second;
    struct linked_list *next;
}elem;


typedef struct result_list{
    float *result; //pointer for conserve a result
    struct result_list *next_result;
}result;


float *vector_operation(char command, int size, float *vector1, float *vector2){
    float *vector; // name a vector. return it as parameter
    if (command != '*'){
        vector = malloc(size * sizeof(float)); // allocate memory for vector length
        switch(command) {
            case '+':
                for (int i = 0; i < size; i++) {vector[i] = vector1[i] + vector2[i];}
                return vector;
            case '-':
                for (int i = 0; i < size; i++) {vector[i] = vector1[i] - vector2[i];}
                return vector;
        }
    }
    else{
        vector = malloc(1 * sizeof(float)); //for scalar multiplication we need only 1 cell
        vector[0] = 0; //for start it's result will be 0
        for (int i = 0; i < size; i++) {vector[0] = vector[0] + (vector1[i] * vector2[i]);}
        return vector;
    }
} //VECTOR OPERATIONS


float *numb_operation(char command, float *first, float *second){
    float y; //support variable for calculation of factorial
    float *support; //return pointer with result
    support = malloc(1 * sizeof(float)); //for result allocate 1 cell
    switch (command){
        case '+':
            support[0] = first[0] + second[0];
            return support;
        case '-':
            support[0] = first[0] - second[0];
            return support;
        case '*':
            support[0] = first[0] * second[0];
            return support;
        case '^':
            support[0] = first[0];
            if (second[0] != 0){
                for (int i = 1; i < second[0]; i++){
                    first[0] = first[0] + support[0];
                }
                return first;
            } else{
                first[0] = 1;
                return first;
            }

        case '/':
            support[0] = first[0] / second[0];
            return support;
        case '!':
            y = 1;
            for(int i = 1; i<=first[0]; i++){y = y * i;}
            support[0] = y;
            return support;
    }
} //SCALAR

//function for reading number by pointer
float *addNumb(FILE *input, int size){
    float *numb;
    numb = malloc(size * sizeof(float));
    for(int i = 0;i < size;i++){fscanf(input,"%f", &numb[i]);}
    return numb;
}


void addelement(elem *current, FILE *input){
    elem *tmp = malloc(1 * sizeof(elem));
    fscanf(input, " %c %c", &tmp->command, &tmp->operation);
    //add in structure
    if (tmp->operation == 'v') { fscanf(input, " %i", &tmp->size);}
    else { tmp->size = 1; }
    if (tmp->command != '!') {
        tmp->first = addNumb(input, tmp->size);
        tmp->second = addNumb(input, tmp->size);
    } else {
        tmp->first = addNumb(input, tmp->size);
        tmp->second = NULL;
    }
    tmp->next = NULL;
    current->next = tmp;
}


void addelement_res(result *current_res, elem *current){
    result *tmp_res = malloc(1 * sizeof(result));
    if (current->operation == 'v'){tmp_res->result = vector_operation(current->command, current->size, current->first, current->second );}
    else{tmp_res->result = numb_operation(current->command, current->first, current->second);}
    tmp_res->next_result = NULL;
    current_res->next_result = tmp_res;
}


int main() {
    FILE *input, *output;
    elem *head, *current; //pointer for list with a data. on a start and current element
    result *head_res, *current_res; //also for list with answers
    input = fopen("input.txt", "r"); //open file in the project folder
    if (!feof(input)) {
        head = malloc(1 * sizeof(elem)); //allocate memory for a head of list (first element)
        fscanf(input, " %c %c", &head->command, &head->operation);
        //write separately data for 1 element
        if (head->operation == 'v') { fscanf(input, " %i", &head->size); }
        else { head->size = 1; }
        if (head->command != '!') {
            head->first = addNumb(input, head->size);
            head->second = addNumb(input, head->size);
        } else {
            head->first = addNumb(input, head->size);
            head->second = NULL;
        }
//copy pointers and add elements until the file isn't over
        current = head;
        while (!feof(input)) {
            addelement(current, input);
            current = current->next;
        }
//create the head of list of answers. allocate memory and place in the function next addition
        head_res = malloc(sizeof(result));
        current = head;
        //similar to the previous list
        if (current->operation == 'v') {
            head_res->result = vector_operation(current->command, current->size, current->first, current->second);
        }
        else { head_res->result = numb_operation(current->command, current->first, current->second); }
        head_res->next_result = NULL;
        current = current->next;
        current_res = head_res;
        while (current != NULL) {
            addelement_res(current_res, current);
            current = current->next;
            current_res = current_res->next_result;
        }
//copy pointers for addition in the file
        current = head;
        current_res = head_res;
        fclose(input);
        output = fopen("output.txt", "w");
        while (current != NULL) {
            if (current->operation == 'v') {
                fprintf(output, "{");
                //output data from 'first', 'second' and 'result'
                for (int i = 0; i < current->size; i++) {
                    fprintf(output, " %.2f ", current->first[i]);
                }
                fprintf(output, "} %c {", current->command);
                for (int i = 0; i < current->size; i++) {
                    fprintf(output, " %.2f ", current->second[i]);
                }
                fprintf(output, "} = ");
                if (current->command != '*') {
                    fprintf(output, "{");
                    for (int i = 0; i < current->size; i++) { fprintf(output, " %.2f ", current_res->result[i]); }
                    fprintf(output, "}\n");
                } else {
                    fprintf(output, " %.2f\n", current_res->result[0]);
                }
            } else {
                fprintf(output, "calculation result: %.2f\n", current_res->result[0]);
           }
//move pointers
            current = current->next;
            current_res = current_res->next_result;
        }
        fclose(output);
    }
    return 0;
}

