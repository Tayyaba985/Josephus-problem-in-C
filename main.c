/*********************************************************************
 This program simulates the Josephus Problem which is text based using
 Doubly Circular linked list

 Author: Abdul Basit Qureshi    FA21-BCE-003
         Tayyaba Saif           FA21-BCE-093
         Aayan Khattak          FA21-BCE-001

 Date started:  14th October 2022
 Last Modified: 22nd October 2022   by Abdul Basit Qureshi
*********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "Josephus.h"
#include "Node.h"

int main()
{
    struct node * head = NULL;
    FILE *fptr;


    fptr = fopen("names.txt","r");      //opening file
    if(fptr == NULL)
    {
        printf("Sorry! Error in opening file.\n");
        exit(1);
    }

    int arr[totalNames];
    srand((unsigned int)time(0));

    ///generating random array of size 100 without repeating any number
    for(int i=0; i<totalNames; i++)
    {
        int tmp = rand()%totalNames;      ///from 0 to 99

        while(i != 0 && isAlreadyAdded(arr, tmp, i))    ///if number is already added then move to next number
            tmp = rand()%totalNames;

        arr[i] = tmp;
    }



    int len, index = 0, lineIndex[totalNames];
    char name[20];

    ///storing starting index of each line from file
    for(int i=0; i<totalNames; i++)
    {
        readNameFromFile(index, name, fptr);
        len = strlen(name);

        index = index + len+1;
        lineIndex[i] = index;   ///storing offset of each line in array
    }




    int people, start_point, skipNum;
    char direction;


    printf("\n  Welcome to the Josephus Problem Program.");

    ///asking for total number of peoples
    do
    {
        printf("\n  Enter the total number of people (max %d): ", totalNames);
        scanf("%d", &people);
        fflush(stdin);
    }while(people > totalNames || people < 0);



    ///inserting in a doubly circular linked list after reading names randomly from file
    for(int i=0; i<people; i++)
    {
        readNameFromFile(lineIndex[arr[i]], name, fptr);

        if(i == 0)    ///will execute in only first iteration
            insertNodeAtStart(&head, &name, i+1);
        else
            insertNodeAfter(head, i-1, &name, i+1);
    }
    fclose(fptr);   //closing file

    ///boundary cases if total number of people is 0 or 1
    if(people == 0)
    {
        printf("\n  No one will left!");
        exit(0);
    }
    else if(people == 1)
    {
        printf("\n  Winning person is\n");
        printClockwise(head);
        exit(0);
    }


    ///asking for starting point
    do
    {
        printf("\n  Enter starting point(1 to %d): ", people);
        scanf("%d", &start_point);
        fflush(stdin);
    }while(start_point < 1 || start_point > people);

    while(head->data.num != start_point)        ///changing starting point
        head = head->next;



    ///asking for the person to be skipped
    do
    {
        printf("\n  Enter number of person to be skipped (2 to %d): ",people);
        scanf("%d", &skipNum);
        fflush(stdin);
    }while(skipNum < 2 || skipNum > people);


    ///asking for direction
    do
    {
        printf("\n  Enter direction clockwise/anti-clockwise (c or a): ");
        direction = getchar();
        fflush(stdin);
    }while(direction != 'a' && direction != 'c');



    ///printing all persons with names according to conditions
    if(direction == 'a')
        printAnticlockwise(head);
    else
        printClockwise(head);

    printf("  are people sitting in a Circle.\n\n");





    int peopleCopy = people;
    int round = 1;
    struct node * temp = head;

    do
    {

        if(direction == 'c')        ///when direction is clockwise
        {
            int flag = 0;

            for(int i=1; i<getListLength(head); i++)
            {
                ///if the total number of people is even then the entire loop will execute one more time
                ///as given condition of loop is for odd numbers
                if(getListLength(head)%2 == 0 && flag == 0)
                {
                    flag = 1;
                    i--;
                }

                //will execute in only first iteration of do while loop
                if(peopleCopy == people)
                {
                    for(int x=0; x<skipNum-1; x++)
                    temp = temp->next;
                }
                else
                {
                    for(int j=1; j<=skipNum; j++)   //moving to the index
                        temp = temp->next;
                }

                if(temp == head)    //if deleting node is  head
                {
                    struct node * tail = head->prev;
                    head = head->next;
                    head->prev = tail;
                    tail->next = head;
                    free(temp);
                }
                else        //if deleting node is other than head
                {
                    struct node * before = temp->prev;
                    struct node * aft = temp->next;
                    before->next = aft;
                    aft->prev = before;
                    free(temp);
                }

                people--;   //minus one from total people every time after removing one person from the list

            }
        }

        else    ///when direction is anti-clockwise
        {
            int flag = 0;

            for(int i=1; i<getListLength(head); i++)
            {

                ///if the total number of people is even then the entire loop will execute one more time
                if(getListLength(head)%2 == 0 && flag == 0)
                {
                    flag = 1;
                    i--;
                }

                if(peopleCopy == people)  //will execute in only first iteration of do while loop
                {
                    for(int x=0; x<skipNum-1; x++)
                    temp = temp->prev;
                }
                else
                {
                    for(int j=1; j<=skipNum; j++)   //moving to the index
                        temp = temp->prev;
                }

                if(temp == head)    //if deleting node is head
                {
                    struct node * tail = head->next;
                    head = head->prev;
                    head->next = tail;
                    tail->prev = head;
                    free(temp);
                }

                else    //if deleting node is other than head
                {
                    struct node * before = temp->next;
                    struct node * aft = temp->prev;
                    before->prev = aft;
                    aft->next = before;
                    free(temp);
                }

                people--;   //minus one from total people every time after removing one person from the list
            }

        }

        ///printing remaining persons after every round
        if(direction == 'c')
        {
            printf("\n  People after %d iteration\n", round);
            printClockwise(head);
        }
        else
        {
            printf("\n  People after %d iteration\n", round);
            printAnticlockwise(head);
        }
        printf("\n\n");
        round++;

    }
    while(people != 1);

    printf("\n  So the winning person is\n");
    printClockwise(head);
    getch();

    printf("\n  Exiting the program!\n");
    return 0;
}
