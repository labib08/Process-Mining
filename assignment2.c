/* Program to discover a process model from an event log.

  Skeleton program written by Artem Polyvyanyy, artem.polyvyanyy@unimelb.edu.au,
  August 2022, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: [Itmam Khan Labib, 1319974]
  Dated:     [11/10/2022]

*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

/* #DEFINE'S -----------------------------------------------------------------*/
#define GOOD_LUCK   "GOOD LUCK CLASS!!!\n"      // good luck message
#define MAXVALS 1000
/* TYPE DEFINITIONS ----------------------------------------------------------*/
typedef unsigned int action_t;  // an action is identified by an integer

typedef struct event event_t;   // an event ...
struct event {                  // ... is composed of ...
    action_t actn;              // ... an action that triggered it and ...
    event_t* next;              // ... a pointer to the next event in the trace
};

typedef struct {                // a trace is a linked list of events
    event_t* head;              // a pointer to the first event in this trace
    event_t* foot;              // a pointer to the last event in this trace
    int      freq;              // the number of times this trace was observed
    int       ind;              // number of traces in this event
    int      obvs;              /* a flag variable to determine whether the
                                   the trace was repeated or not*/
} trace_t;

typedef struct {                // an event log is an array of distinct traces
                                //     sorted lexicographically
    trace_t* trcs;              // an array of traces
    
} log_t;

typedef struct sequence sequence_t;
struct sequence {
    action_t     actn1;
    action_t     actn2;
    int           frqn;
    sequence_t*   next;
};

typedef struct {
    sequence_t* hd;
    sequence_t* ft;
} list_t;

typedef action_t** DF_t;        // a directly follows relation over actions

/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
char get_event();
trace_t *insert_at_foot(trace_t *trace, action_t event);
trace_t* make_empty_trace(void);
int is_empty_trace(trace_t* trace);
action_t get_head(trace_t* trace);
int get_distinct_events(log_t log[], action_t A[], int B[], int n);
void free_list(list_t* list); 
int check_repetitive_event(action_t A[], int B[], action_t action, int n);
void count_repetitve_event(int B[], int n);
void sort_distinct_events(action_t A[], int B[], int n);
int most_frequent_trace(log_t log[], int n, int* count);
int freq_trace(event_t* nxt1, event_t* nxt2);
void print_stage_0(int tot_distinct_events, int count, int totevents,
    int tot_traces, int freq_trace, action_t distinct_events[], 
    int distinct_events_freq[], log_t log[]);
void make_sequential_list(log_t log[], list_t* list, int n);
list_t* make_empty_list(void);
list_t* list_insert_at_head(list_t* list, action_t action1, action_t action2);
list_t* list_insert_at_foot(list_t* list, action_t action1, action_t action2);
int check_sequence(list_t* list, action_t action1, action_t action2);
int get_sequence_num(list_t* list, action_t action1, 
                     action_t action2, int curr);
void print_stage_1_and_2(list_t* list, log_t log[], action_t distinct_events[], 
    int distinct_events_freq[], int n, int tot_traces, int totevents);
void change_trace_and_list(list_t* list, log_t log[], int n,
    action_t action1, action_t action2, action_t spec); 
void delete_node(log_t log[], int n, action_t spec);
void change_distinct_event(action_t distinct_events[], 
    int distinct_events_freq[], int n, action_t action1, action_t action2,
    int num, action_t spec, int max);
int check_sequence_pattern(list_t* list, action_t distinct_events[],
        int distinct_events_freq[], int n);
void print_stage_2(list_t* list, log_t log[], action_t distinct_events[], 
    int distinct_events_freq[], int n, int tot_traces, action_t spec,
    action_t x, int totevents);
void print_table(action_t distinct_events[], int n, action_t x);
int get_weight(list_t* list, action_t action1, action_t action2, int*p);
int check_CHC(list_t* list, action_t action1, action_t action2, int totevents);
int check_alpha(action_t distinct_events[], int n);
void print_pattern(action_t spec, action_t action1, action_t action2,
                   int flag2, int flag4);
void free_trace(trace_t* trace);

int
main(int argc, char *argv[]) {
    log_t log[MAXVALS];
    action_t event;
    trace_t *trace;
    int count=0;
    int totevents=0;
    int tot_distinct_events, freq_trace;
    action_t distinct_events[MAXVALS];
    int distinct_events_freq[MAXVALS];
    int tot_traces=0;
    list_t* list;
    list = make_empty_list();
    trace = make_empty_trace();
    /*Loop through each event and form a trace, then add the trace
      in the event log to form an array of log*/
    while ((event = get_event()) != EOF){
        if (isalpha(event)){
            trace = insert_at_foot(trace, event);
            totevents++;
            //printf("%c", event);
        }
        else{ 
            log[tot_traces].trcs = trace;
            tot_traces++;
            trace = make_empty_trace();
        }
    }
    log[tot_traces].trcs = trace;
    tot_traces++;
    //Find the total number of distinct events
    tot_distinct_events = get_distinct_events(log, distinct_events,
        distinct_events_freq, tot_traces);
    //Sort the array of distinct events    
    sort_distinct_events(distinct_events, distinct_events_freq,
        tot_distinct_events);
        
    //Find the most frequent trace in the event log
    freq_trace = most_frequent_trace(log, tot_traces, &count);
    //Print stage 0
    print_stage_0(tot_distinct_events, count, totevents, tot_traces, freq_trace,
        distinct_events, distinct_events_freq, log);
    //Make a list of sequential pattern    
    make_sequential_list(log, list, tot_traces);
    //Print Stage 1 and 2
    print_stage_1_and_2(list, log, distinct_events, distinct_events_freq, 
        tot_distinct_events, tot_traces, totevents);
    //Free the list and log of traces    
    free_list(list);
    for (int i=0; i< tot_traces; i++){
        free_trace(log[i].trcs);
    }
    return EXIT_SUCCESS;        
}
/* Function to get individual events for a particular trace, until the end
   of line or end of file is reached */
char get_event(){
    int c;
    while ((c =getchar()) != EOF){
        if (c == '\n')
            return 1;
        if (isalpha(c))
            return c;
        
    }
    return EOF;
}
// Function to make an empty trace
trace_t* make_empty_trace(void){
    trace_t* trace;
    trace = (trace_t*)malloc(sizeof(*trace));
    trace -> head = trace -> foot = NULL;
    return trace;
}

// Function takes a new event and inserts at the end of the particular trace
trace_t *insert_at_foot(trace_t* trace, action_t event){
    event_t* new;
    new = (event_t*)malloc(sizeof(*new));
    new -> actn = event;
    new -> next = NULL;
    if (trace -> foot == NULL){
        trace -> foot = trace -> head = new;
    }
    else{
        trace -> foot -> next = new;
        trace -> foot = new;
    }
    return trace;
}

int is_empty_trace(trace_t *trace){
    return trace -> head == NULL;
}

action_t get_head(trace_t* trace){
    return trace -> head -> actn;
}

/* Computes the distinct events from the log of events and returns
   the total number*/
int get_distinct_events(log_t log[], action_t A[], int B[], int n){
    event_t* nxt;
    int index =0;
    int flag, ind;
    action_t action;
    for (int i=0; i<n; i++){
        nxt = log[i].trcs->head;
        ind =0;
        while (nxt != NULL){
            flag =0;
            action = nxt -> actn;
            // Function to check whether the event was seen before
            flag = check_repetitive_event(A, B, action, index);
            // If not seen, insert it in the array
            if (flag){
                A[index] = nxt -> actn;
                index++;
            }
            ind++;
            nxt = nxt -> next;
        }
        log[i].trcs->ind =ind;
    }
    //Count the total number of events
    count_repetitve_event(B, index);
    return index;
}
void count_repetitve_event(int B[], int n){
    for (int i=0; i<n; i++){
        B[i] += 1;
    }
}
// Check whether any event was repeated
int check_repetitive_event(action_t A[], int B[], action_t action, int n){
    int actn_present =0;
    int count=1;
    for (int i=0; i< n; i++){
        if (A[i] == action){
            actn_present =1;
            B[i] += count;
            break;
        }
    }
    // If there was a repeat return False
    if (actn_present)
        return 0;
    // Else return True
    return 1;
}
// Sort the array of distinct events lexicographically
/* Function was copied and adjusted from the website
   "https://www.tutorialspoint.com/learn_c_by_examples/
   program_to_sort_string_characters_in_c.htm", under the topic implementation*/
void sort_distinct_events(action_t A[], int B[], int n){
    action_t temp;
    int i, j, temp1;
    for (i = 0; i < n-1; i++) {
      for (j = i+1; j < n; j++) {
         if (A[i] > A[j]) {
            temp = A[i];
            temp1 = B[i];
            A[i] = A[j];
            B[i] = B[j];
            A[j] = temp;
            B[j] = temp1;
         }
      }
    }
}
// Finds the most frequent trace
int most_frequent_trace(log_t log[], int n, int* count){
    event_t* nxt1; 
    event_t* nxt2;
    int frequency, flag;
    int j, max;
    max =0;
    // Compares two traces of events at the same time
    for (int i=0; i< n; i++){
        nxt1 = log[i].trcs -> head;
        frequency = log[i].trcs ->freq = 1; 
        for (j=i+1; j<n; j++){
            nxt2 = log[j].trcs -> head;
            flag =0;
            // Checks for the same trace
            flag = freq_trace(nxt1, nxt2);
            // If same trace found, frequency increments
            if (flag){
                frequency++;
                log[i].trcs ->freq =frequency;
                /* Setting the repeated trace with a variable of -1 so we
                   do not count it again in the future*/
                log[j].trcs ->obvs =-1;
            }
        }
    }
    // Finds the most frequent trace
    for (int k=0; k<n; k++){
        frequency = log[k].trcs ->freq;
        if (frequency > max)
            max = frequency;
    }
    // Counts the total number of distinct traces
    for (int l=0; l<n;l++){
        if (log[l].trcs->obvs == 0){
            *count=*count+1;
        }
    }
    return max;
}
// Checks for repeated trace
int freq_trace(event_t* nxt1, event_t* nxt2){
    while(nxt1 != NULL && nxt2 != NULL) {
        if(nxt1->actn != nxt2->actn) 
            return 0;
        nxt1 = nxt1->next;
        nxt2 = nxt2->next;
    }
    if (nxt1 != NULL)
        return 0;
    if (nxt2 != NULL)
        return 0;
    return 1;
}
// Prints stage 0
void print_stage_0(int tot_distinct_events, int count, int totevents,
    int tot_traces, int freq_trace, action_t distinct_events[], 
    int distinct_events_freq[], log_t log[]){
    event_t* nxt;
    printf("==STAGE 0============================\n");
    printf("Number of distinct events: %d\n", tot_distinct_events);
    printf("Number of distinct traces: %d\n", count);
    printf("Total number of events: %d\n", totevents);
    printf("Total number of traces: %d\n", tot_traces);
    printf("Most frequent trace frequency: %d\n", freq_trace);
    // Only prints the traces with the highest number of frequency
    for (int k=0; k< tot_traces; k++){
        if (log[k].trcs->freq == freq_trace){
            nxt = log[k].trcs->head;
            while (nxt != NULL){
                printf("%c", nxt->actn);
                nxt = nxt -> next;
            }
            printf("\n");
        }
    }
    for (int j=0; j< tot_distinct_events; j++){
        printf("%c = %d\n", distinct_events[j], distinct_events_freq[j]);
    }
}
/* Makes a list of consecutive occuring events */
   
void make_sequential_list(log_t log[], list_t* list, int n){
    event_t* nxt;
    action_t action1;
    action_t action2;
    int flag;
    for (int i=0; i<n; i++){
        nxt = log[i].trcs->head;
        while (nxt -> next != NULL){
            flag =0;
            action1 = nxt -> actn;
            action2 = nxt-> next -> actn;
            // Checks whether the pair of events was seen before
            flag = check_sequence(list, action1, action2);
            // If not, add it to the list
            if (!flag){
                list = list_insert_at_foot(list, action1, action2);
            }
            nxt = nxt -> next;
        }
    }
}
// Make an empty list
list_t* make_empty_list(void){
    list_t* list; 
    list = (list_t*)malloc(sizeof(*list));
    list -> hd = list -> ft = NULL;
    return list;
}
/* Function to insert the pair of consecutive occuring events at the head 
   of the list */
list_t* list_insert_at_head(list_t* list, action_t action1, action_t action2){
    sequence_t* new;
    new = (sequence_t*)malloc(sizeof(*new));
    new -> actn1 = action1;
    new -> actn2 = action2;
    new -> frqn = 1;
    new -> next = list -> hd;
    list -> hd = new;
    if (list -> ft == NULL){
        list -> ft = new;
    }
    return list;
}
/* Function to insert the pair of consecutive occuring events at the head 
   of the list. This function is implemented from listops.c */
list_t* list_insert_at_foot(list_t* list, action_t action1, action_t action2){
    sequence_t* new;
    new = (sequence_t*)malloc(sizeof(*new));
    new -> actn1 = action1;
    new -> actn2 = action2;
    new -> frqn = 1;
    if (list -> ft == NULL){
        list -> hd = list -> ft = new;
    }
    else{
        list -> ft -> next = new;
        list -> ft = new;
    }
    return list;
}
/* Function to detect repeated pair of events and increases their
   frequency */
int check_sequence(list_t* list, action_t action1, action_t action2){
    sequence_t* nxt;
    nxt = list -> hd;
    while (nxt != NULL){
        if (nxt -> actn1 == action1 && nxt -> actn2 == action2){
            nxt -> frqn++;
            return 1;
        }
        nxt = nxt -> next;
    }
    return 0;
}


void print_stage_1_and_2(list_t* list, log_t log[], action_t distinct_events[], 
    int distinct_events_freq[], int n, int tot_traces, int totevents){
    int num, max, var;
    int check =1;
    action_t x =-1;
    action_t action1, action2;
    int new_freq, flag;
    action_t spec = 256;
    int sequence_pattern =1;
    int same_num =0;
    printf("==STAGE 1============================\n");
    /* Keeps on printing the table until pd of two pair of events is found to
       be less than 70*/
    while (sequence_pattern ==1){
        max =0;
        new_freq=0;
        // If the two events have a pd of less than the 70, the loop breaks
        if ((var = check_sequence_pattern(list, distinct_events, distinct_events_freq, n))==0){
            print_stage_2(list, log, distinct_events, distinct_events_freq,
                n, tot_traces, spec, x, totevents);
            sequence_pattern =0;
            break;
        }
        if (check>1){
            printf("=====================================\n");
        }
        // Function to print the first row
        print_table(distinct_events, n, x);
        printf("\n");
        for (int i=0; i< n; i++){
            flag =1;
            for (int j=0; j<n; j++){
                if (distinct_events[i] != x && distinct_events[j] != x){
                    // Obtains the frequency of the two pair of events
                    num = get_sequence_num(list, distinct_events[i], distinct_events[j], 1);
                    // Flag variable to determine the end of line
                    if (flag){
                        // Checks whether the action is an integer or a character
                        if (isalpha(distinct_events[i])){    
                            printf("%5c", distinct_events[i]);
                        }
                        else
                            printf("%5d", distinct_events[i]);    
                        flag =0;
                    }
                  
                    if (distinct_events[i] != distinct_events[j]){
                        printf("%5d", num);
                    }
                    else{
                        printf("%5d", same_num);
                    }
                    // Records the maximum number and the two events that caused it
                    if(isalpha(distinct_events[i]) && isalpha(distinct_events[j])){
                        if (num > max){
                            max = num;
                            action1 = distinct_events[i];
                            action2 = distinct_events[j];
                        }
                    }
                }
            }
            printf("\n");
        }
        // Prints the events and the number of items to be removed
        printf("-------------------------------------\n");
        printf("%d = SEQ(%c,%c)\n", spec, action1, action2);
        printf("Number of events removed: %d\n", max);
        // Finds the sum of frequencies of the two evens
        for (int i=0; i< n; i++){
            if(distinct_events[i] == action1){
                new_freq += distinct_events_freq[i];
            }
            if(distinct_events[i] == action2){
                new_freq += distinct_events_freq[i];
            }
        }
        /* Makes new changes to the array of distinct events, as the traces 
           are being removed */ 
        change_distinct_event(distinct_events, distinct_events_freq, n, action1,
            action2, new_freq, spec, max);
        // Changes to the array of traces and list are also made
        change_trace_and_list(list, log, tot_traces, action1, action2, spec);
        // The old list is freed and a new list is made with the new events
        free(list);
        list = make_empty_list();
        make_sequential_list(log, list, tot_traces);
        spec++;
        check++;
        n--;
    }
    return;
}
// Function that changes the trace and list and adds the new sequence
void change_trace_and_list(list_t* list, log_t log[], int n,
    action_t action1, action_t action2, action_t spec){
    event_t* nxt;
    for (int i=0; i<n; i++){
        nxt = log[i].trcs->head;
        
        while (nxt != NULL){
            if ((nxt -> actn == action1) || (nxt -> actn == action2)) {
                nxt -> actn = spec;   
            }
            nxt = nxt -> next;
        }
    }
    return;
}

/* Changes and prints the array of distinct events and its consecutive
   frequencies */
void change_distinct_event(action_t distinct_events[], 
    int distinct_events_freq[], int n, action_t action1, action_t action2,
    int num, action_t spec, int max){
    action_t x = -1;
    int y = -1;
    // Finds the events and makes changes to it
    for(int i=0; i<n; i++){
        if(distinct_events[i] == action1){
            distinct_events[i] = spec;
            distinct_events_freq[i] = num - max;
        }
        if (distinct_events[i] == action2){
            distinct_events[i] = x;
            distinct_events_freq[i] = x;
        }
    }
    // sorts the array again
    sort_distinct_events(distinct_events, distinct_events_freq, n);
    // prints the events and their frequencies
    for (int i=0; i<n; i++){
        if (distinct_events_freq[i] != y){
            if (isalpha(distinct_events[i]))
                printf("%c = %d\n", distinct_events[i], distinct_events_freq[i]);
            else
                printf("%d = %d\n", distinct_events[i], distinct_events_freq[i]);    
        }
    }   
    return;
}
// Checks whether the pd of the consecutive event is less or greater than 70
int check_sequence_pattern(list_t* list, action_t distinct_events[],
    int distinct_events_freq[], int n){
    action_t action1, action2;
    sequence_t* nxt;
    int pd, num, max;
    int val1 =0;
    int val2 =0;
    max =0;
    
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            if(isalpha(distinct_events[i]) && isalpha(distinct_events[i])){
                num = get_sequence_num(list, distinct_events[i], distinct_events[j], 1);
                if (num > max){
                    max = num;
                    action1 = distinct_events[i];
                    action2 = distinct_events[j];
                }
            }
        }
    }
    nxt = list -> hd;
    while (nxt != NULL){
        if (action1 != action2){
            if (nxt -> actn1 == action1 && nxt -> actn2 == action2){
                val1 = nxt -> frqn;
            }
            if (nxt -> actn1 == action2 && nxt -> actn2 == action1){
                val2 = nxt -> frqn;
            }
        }
        nxt = nxt -> next;
    }
    if (val1 > val2){
        pd = (100*(val1 - val2))/val1;
        if (pd > 70)
            return 1;
        else
            return 0;
    }
    return 0;
}
// Prints stage 2
void print_stage_2(list_t* list, log_t log[], action_t distinct_events[], 
    int distinct_events_freq[], int n, int tot_traces, action_t spec,
    action_t x, int totevents){
    int flag, num, max, weight, num_rem, same_num;
    printf("==STAGE 2============================\n");
    int new_num, new_freq;
    int check =1;
    action_t action1, action2;
    int flag1, flag2, flag3, flag4, CHC; 
    while (n>1){
        max =0, new_num =0, new_freq =0, same_num =0;
        CHC =0;
        flag1 =0;
        flag2 =1;
        flag3 = flag4 =0;
        if (check>1){
            printf("=====================================\n");
        }
        print_table(distinct_events, n, x);
        printf("\n");
        // Checks if there is any character left in the array
        flag1 = check_alpha(distinct_events, n);
        for(int i =0; i< n; i++){
            flag =1;
            for (int j=0; j< n; j++){
                if (distinct_events [i] != x && distinct_events [j] != x){
                    num = get_sequence_num(list, distinct_events[i], distinct_events[j], 1);
                    // Gets the total number of events to be removed
                    num_rem = get_sequence_num(list, distinct_events[i], distinct_events[j], 2);
                    // Calculates the weight of the event
                    weight = get_weight(list, distinct_events[i], distinct_events[j], &flag3);
                    if (flag1)
                        // Checks whether the events to be removed are choice patterns
                        CHC = check_CHC(list, distinct_events[i], distinct_events[j], totevents);
                       
                    if (CHC){
                        flag2 = 0;
                    }
                    if (flag){
                        if (isalpha(distinct_events[i])){
                            printf("%5c", distinct_events[i]);
                        }
                        else
                            printf("%5d", distinct_events[i]);
                        flag =0;
                    }
                    if (distinct_events[i] != distinct_events[j]){
                        printf("%5d", num);
                    }
                    else{
                        printf("%5d", same_num);
                    }
                    // Finds the highest of weight and records the pair of events
                    // responsible for this
                    if (weight > max){
                        max = weight;
                        new_num = num_rem;
                        action1 = distinct_events[i];
                        action2 = distinct_events[j];
                        if (flag3 ==1){
                            flag4 =1;
                        }
                        else{
                            flag4 =2;
                        }
                    }
                    
                }
            }
            printf("\n");
        }
        printf("-------------------------------------\n");
        
        print_pattern(spec, action1, action2, flag2, flag4);
        
        printf("Number of events removed: %d\n", new_num);
        for (int i=0; i< n; i++){
            if(distinct_events[i] == action1){
                new_freq += distinct_events_freq[i];
            }
            if(distinct_events[i] == action2){
                new_freq += distinct_events_freq[i];
            }
        }

        change_distinct_event(distinct_events, distinct_events_freq, n, action1,
            action2, new_freq, spec, new_num);
        change_trace_and_list(list, log, tot_traces, action1, action2, spec);
        //delete_node(log, tot_traces, spec);
        free(list);
        list = make_empty_list();
        make_sequential_list(log, list, tot_traces);
        spec++;
        check++;
        n--;
    }
    printf("==THE END============================\n");
    return;
}
// Prints the pair of events to be removed
void print_pattern(action_t spec, action_t action1, action_t action2,
                   int flag2, int flag4){
    if (!flag2){
        if (isalpha(action1) && isalpha(action2))
            printf("%d = CHC(%c,%c)\n", spec, action1, action2);
        if (isalpha(action1) && !isalpha(action2))
             printf("%d = CHC(%c,%d)\n", spec, action1, action2);
        if (!isalpha(action1) && isalpha(action2))
            printf("%d = CHC(%d,%c)\n", spec, action1, action2);
        if (!isalpha(action1) && !isalpha(action2))
            printf("%d = CHC(%d,%d)\n", spec, action1, action2);
    }
    else{
        if(flag4 == 2){
            
            if (isalpha(action1) && isalpha(action2))
                printf("%d = CON(%c,%c)\n", spec, action1, action2);
            if (isalpha(action1) && !isalpha(action2))
                 printf("%d = CON(%c,%d)\n", spec, action1, action2);
            if (!isalpha(action1) && isalpha(action2))
                printf("%d = CON(%d,%c)\n", spec, action1, action2);
            if (!isalpha(action1) && !isalpha(action2))
                printf("%d = CON(%d,%d)\n", spec, action1, action2);
        }
        if (flag4 == 1){
            printf("%d = SEQ(%d,%d)\n", spec, action1, action2);    
        }
    }
    return;
}
// Prints the first row of the table 
void print_table(action_t distinct_events[], int n, action_t x){
    for (int k=0; k< n; k++){
        if (distinct_events[k] != x){
            if (k==0){
                if(isalpha(distinct_events[k])){
                    printf("%10c", distinct_events[k]);
                }
                else
                    printf("%10d", distinct_events[k]);
            }
            else{
                if(isalpha(distinct_events[k])){
                    printf("%5c", distinct_events[k]);
                }
                else
                    printf("%5d", distinct_events[k]);
            }
        }
    }
    return;
}
// Gets the total frequency of the concurring events
int get_sequence_num(list_t* list, action_t action1,
                     action_t action2, int curr){
    sequence_t* nxt;
    int val =0;
    nxt = list -> hd;
    while (nxt != NULL){
        if (curr ==1){
            if (nxt -> actn1 == action1 && nxt -> actn2 == action2){
                return nxt -> frqn;    
            }
        }
        if (curr ==2){
            if (nxt -> actn1 == action1 && nxt -> actn2 == action2){
                val += nxt -> frqn;
            }
            if (nxt -> actn1 == action2 && nxt -> actn2 == action1){
                val += nxt -> frqn;
            }
        }
        nxt = nxt -> next;
    }
    return val;
}
// Calculates the weight of the two pair of events
int get_weight(list_t* list, action_t action1, action_t action2, int *p){
    sequence_t* nxt;
    int val1, val2, pd, weight;
    int seq_weight, con_weight;
    seq_weight = con_weight = weight =0;
    pd =0;
    val1=1;
    val2=1;
    nxt = list -> hd;
    if (action1 == action2)
        return 0;
    while (nxt != NULL){
        if (nxt -> actn1 == action1 && nxt -> actn2 == action2){
            val1 = nxt -> frqn;
        }
        if (nxt -> actn1 == action2 && nxt -> actn2 == action1){
            val2 = nxt -> frqn;
        }

    nxt = nxt -> next;
    }
    // Finds the pd first
    pd = (100*abs(val1 -val2));
    if (val1>val2)
        pd = pd / val1;
    else
        pd = pd / val2;
    // Then calculates the weight
    if (val1> val2){
        weight = abs(50 - pd) * val1;
    }
    else
        weight = abs(50 - pd) * val2;
      
    if (pd <30){
        con_weight = 100*weight;
    }
    else{
        
        seq_weight = weight;
    }
    if (con_weight > seq_weight){
        *p =2;
        return con_weight;
    }
    else{
        *p =1;
        return seq_weight;
    }
    return 0;
}
// Checks for candidate choice pattern
int check_CHC(list_t* list, action_t action1, action_t action2, int totevents){
    sequence_t* nxt;
    int val1, val2;
    val1 = val2 =0;
    nxt = list -> hd;
    if (action1 == action2)
        return 0;
    while (nxt != NULL){
        if (nxt -> actn1 == action1 && nxt -> actn2 == action2){
            val1 = nxt -> frqn;
        }
        if (nxt -> actn1 == action2 && nxt -> actn2 == action1){
            val2 = nxt -> frqn;
        }
        nxt = nxt -> next;
    }

    if (val1>val2){
        if (val1 <= (totevents/100)){
            return 1;
        }
    }
    else{
        if (val2 <= (totevents/100)){
            return 1;
        }
    }
    return 0;
}
// Checks whether any characters left in the array
int check_alpha(action_t distinct_events[], int n){
    for (int i=0; i<n; i++){
        if (isalpha(distinct_events[i])){
            return 0;
        }
    }
    return 1;
}
// Frees the list. Implemented from listops.c
void free_list(list_t* list){
    sequence_t* curr;
    sequence_t* prev;
    curr = list -> hd;
    while (curr){
        prev = curr;
        curr = curr->next;
        free(prev);
    }
    free(list);
}
// Frees the trace. Implemented from listops.c
void free_trace(trace_t* trace){
    event_t* curr;
    event_t* prev;
    curr = trace -> head;
    while (curr){
        prev = curr;
        curr = curr->next;
        free(prev);
    }
    free(trace);
}






