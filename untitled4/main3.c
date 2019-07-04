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
    char operationS, operationV;
    int size;
    float *first, *second;
    struct linked_list *next;
}elem;


typedef struct result_list{
    float *result;
    struct result_list *next_result;
}result;


float *vector_operation(char command, int size, float *vector1, float *vector2){
    float *vector;
    if (command != '*'){
        vector = malloc(size * sizeof(float));
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
        vector = malloc(1 * sizeof(float));
        vector[0] = 0;
        for (int i = 0; i < size; i++) {vector[0] = vector[0] + (vector1[i] * vector2[i]);}
        return vector;
    }
} //VECTOR


float *numb_operation(char command, float *first, float *second){
    float y;
    float *support;
    support = malloc(1 * sizeof(float));
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
    fscanf(input, " %c %c", &tmp->operationS, &tmp->operationV);
    if (tmp->operationV == 'v') { fscanf(input, " %i", &tmp->size);}
    else { tmp->size = 1; }
    if (tmp->operationS != '!') {
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
    if (current->operationV == 'v'){tmp_res->result = vector_operation(current->operationS, current->size, current->first, current->second );}
    else{tmp_res->result = numb_operation(current->operationS, current->first, current->second);}
    tmp_res->next_result = NULL;
    current_res->next_result = tmp_res;
}


int main() {
    FILE *input, *output;
    elem *head, *current;
    result *head_res, *current_res;
    input = fopen("input.txt", "r");
    if (!feof(input)) {
        head = malloc(1 * sizeof(elem));
        fscanf(input, " %c %c", &head->operationS, &head->operationV);
        if (head->operationV == 'v') { fscanf(input, " %i", &head->size); }
        else { head->size = 1; }
        if (head->operationS != '!') {
            head->first = addNumb(input, head->size);
            head->second = addNumb(input, head->size);
        } else {
            head->first = addNumb(input, head->size);
            head->second = NULL;
        }

        current = head;
        while (!feof(input)) {
            addelement(current, input);
            current = current->next;
        }

        head_res = malloc(sizeof(result));
        current = head;
        if (current->operationV == 'v') {
            head_res->result = vector_operation(current->operationS, current->size, current->first, current->second);
        }
        else { head_res->result = numb_operation(current->operationS, current->first, current->second); }
        head_res->next_result = NULL;
        current = current->next;
        current_res = head_res;
        while (current != NULL) {
            addelement_res(current_res, current);
            current = current->next;
            current_res = current_res->next_result;
        }

        current = head;
        current_res = head_res;
        fclose(input);
        output = fopen("output.txt", "w");
        while (current != NULL) {
            if (current->operationV == 'v') {
                fprintf(output, "{");
                for (int i = 0; i < current->size; i++) {
                    fprintf(output, " %.2f ", current->first[i]);
                }
                fprintf(output, "} %c {", current->operationS);
                for (int i = 0; i < current->size; i++) {
                    fprintf(output, " %.2f ", current->second[i]);
                }
                fprintf(output, "} = ");
                if (current->operationS != '*') {
                    fprintf(output, "{");
                    for (int i = 0; i < current->size; i++) { fprintf(output, " %.2f ", current_res->result[i]); }
                    fprintf(output, "}\n");
                } else {
                    fprintf(output, " %.2f\n", current_res->result[0]);
                }
            } else {
                fprintf(output, "calculation result: %.2f\n", current_res->result[0]);
           }

            current = current->next;
            current_res = current_res->next_result;
        }
        fclose(output);
    }
    return 0;
}

