#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>

#define SIZEOFIMAGE 784
#define MAXSIZEIMAGES 1000

typedef struct//struct of image
{
    int img_arr [SIZEOFIMAGE];
} image;

typedef struct//struct of data item
{
    image data;
    int Key;
} data_item;

typedef struct//struct of the linear hash table
{
    int hashtable[MAXSIZEIMAGES];
} hash_maplinear;

typedef struct//struct of the node use in linked list
{
    int Data;
    struct Node *next;
} Node;

typedef struct// struct of linked list
{
    Node *Head,*Tail;
} LinkedList;

typedef struct//struct of the hash table of separate chaining
{
    LinkedList Chain[MAXSIZEIMAGES];//array of linked list to used in the hash table every node will point to a linked list
} hash_mapchain;

data_item Images [MAXSIZEIMAGES];//the array to collect all the images when we read from the file

data_item Imagessearch [MAXSIZEIMAGES];// the array to read from the small file which we used in search and delete

hash_maplinear hashmaplinear;

hash_mapchain hashmapchain;

double time_spent_chain;// time taken to insert 1000 images with separate chain

double time_spent_linear;//time taken to insert 1000 images with linear probing

char nameoffile[100];//the string to know the name of the small file to read

int numberofimages = 0;//counter of number of images in the small file

int propingselected = 0;//a flag to know that the file was read and don't read it again

int chainselected = 0;//a flag to know that the file was read and don't read it again

int chains = 0;//to collision chain one time

int propings = 0;//to collision linear one time

int AVAILABLE = -1;//when we delete from the hash table it will be available to know that this place is free and can enter in it a new value

int collisionschain=0;//counter to number of collision when insert 1000 image using separate chain

int collisionslinear=0;//counter to number of collision when insert 1000 image using linear probing

int getlinear(image img);//function to get the id when search with linear probing

int getchain(image img);//function to get the id when search with separate chain

void putlinear(int key, image current_image);//function to put the images that we read in the file in the hash table using linear probing

int removeImagelinear(image img);//functions to remove the image using linear probing

int removeImagechain(image img);//functions to remove the image using separate chain

int hashCode(image img);//functions to calculate and return the hashcode of the image

void ReadFile(char *nameoffile);//functions to read the images from the file

void putchain(int key, image current_image);//function to put the images that we read in the file in the hash table using separate chain

Node *newNode(int data);//function to create a new node

void initLinkedList(LinkedList *list);

void addTail(LinkedList *list,int data);

void chain();//functions that put the images we read from the file in the hash table using separate chaining

void proping();//functions that put the images we read from the file in the hash table using linear probing

void searchlinear();//functions that get the ID from the function get and print it using linear

void removelinear();//functions that get the ID from the function remove and remove the image using linear

void searchchain();//functions that get the ID from the function get and print it using chaining

void removechain();//functions that get the ID from the function remove and remove the image using chaining

Node *newNode(int data)
{
    Node *n = (Node*)malloc(sizeof(Node));
    n->Data=data;
    n->next=NULL;
    return n;
}
void initLinkedList(LinkedList *list)
{
    list->Head=NULL;
    list->Tail=NULL;
}
void addTail(LinkedList *list,int data)
{
    Node *n = newNode(data);
    if(!list->Tail)
    {
        list->Head=n;
        list->Tail=n;
    }
    else if(list->Tail)
    {
        list->Tail->next=n;
        list->Tail=n;
    }
}

int main()
{
    int j=0,k=0;
    int choosenum;
    strcpy(nameoffile,"imagesfile.txt");//to read the 1000 image file and that its name
    ReadFile(nameoffile);//the function readfile you send it the name of the file to read it
    menu();//open the menu
    return 0;
}
void ReadFile(char *nameoffile)
{
    FILE *fptr;
    int j=0,i=0;
    if(strcmp(nameoffile,"imagesfile.txt") == 0)//if it calls the name of the file it enters
    {
        fptr=fopen("imagesfile.txt","r");
        if(fptr==NULL)
        {
            printf("File NOT FOUND!!! \n");
            printf("please enter any key to return to the menu..\n");
            propingselected = 0;//reset a flag that if increased means the file is read and it reads one time to know the number of collision the flag for linear
            chainselected =0;//reset a flag that if increased means the file is read and it reads one time to know the number of collision the flag for chaining
            getch();
            system("cls");
            menu();
        }
        else
        {
            while(!feof(fptr))
            {
                if(j==784)//it reach the last digit of  the image is the id of the image
                {
                    fscanf(fptr,"%d",&(Images[i].Key));
                    j=0;//reset the counter to read the new image
                    i++;//counter of the images
                }
                if(i==1000)break;//means it read the maxsize of file
                fscanf (fptr, "%d", &(Images[i].data.img_arr[j]));
                j++;//to read the pixels
            }
            fclose(fptr);
        }
    }
    else
    {
        fptr=fopen(nameoffile,"r");
        if(fptr==NULL)
        {
            printf("File NOT FOUND!!! \n");
            printf("please enter any key to return to the menu..\n");
            propingselected = 0;
            chainselected =0;
            getch();
            system("cls");
            menu();
        }
        else
        {
            while(!feof(fptr))
            {
                if(j==784)//it didn't have the key so we skip it
                {
                    j=0;
                    i++;
                    numberofimages++;//counter number of images global to the number of images in the small file
                }
                if(i==1000)break;
                fscanf (fptr, "%d", &(Imagessearch[i].data.img_arr[j]));
                j++;
            }
        }
    }
}

int hashCode(image img)
{
    int sum=0;
    int i=1;
    int j=0;
    int hashcodenum = 0;
    for(j=0; j<SIZEOFIMAGE; j++)
    {
        if(j%28 == 0)//when it reaches the end of the row
        {
            sum=sum*i;//it multiply sum with the number of row
            hashcodenum = hashcodenum + sum;//and the hash code is the sum of all the colomn mulitply with row number
            sum = 0;//and then reset the counter to sum the next colomn
            i++;
        }
        sum=sum+img.img_arr[j];//sum all the colomns
    }
    return hashcodenum%1000;//return hashcode modulus 1000 to remain in the size of the hash table
}
void putlinear(int key, image current_image)
{
    int hashcodenum = hashCode(current_image);//return hashcodeimage
    while(hashmaplinear.hashtable[hashcodenum] != NULL && hashmaplinear.hashtable[hashcodenum] != AVAILABLE)//if it's not equal null or available
        // means its not empty and then increase the index of it till it enteran empty place
    {
        hashcodenum++;
        collisionslinear++;//increase the number of collisions happened
    }
    hashmaplinear.hashtable[hashcodenum] = key;//enter the id or the data in the index
}
int getlinear(image img)
{
    int ID;//return the id in the hashtable with the hashcode of the image
    int search;//search index in the hashtable
    int j;
    int i;
    int hashcodeimg = hashCode(img);
    int hashcodeimgstart = hashCode(img);//we will increase it if the id pixels are not equal
    for(j=0; j<MAXSIZEIMAGES; j++)
    {
        ID = hashmaplinear.hashtable[hashcodeimgstart];
        search = hashCode(Images[ID].data);
        if(ID == AVAILABLE)//means it removed
        {
            return 1000;//means image not found
        }
        if(search == hashcodeimg)
        {
            for(i=0; i<SIZEOFIMAGE; i++)//loop that compare the pixels of the image because there are many images with the same hashcode
            {
                if(Images[ID].data.img_arr[i] != img.img_arr[i])
                    break;
            }
            if(i == SIZEOFIMAGE)
                return ID;
        }
        hashcodeimgstart++;//increase the hashcodeimg
        if(hashcodeimgstart == 1000)
            hashcodeimg = 0;//and it reach the max size reset it beacuse it may start from the middle
    }
    if(j==1000)
    {
        return 1000;
    }
}
int removeImagelinear(image img)
{
    int ID;
    int indexhash;
    int j;
    int i;
    int hashcodeimg = hashCode(img);
    int hashcodeimgstart = hashCode(img);
    for(j=0; j<MAXSIZEIMAGES; j++)
    {
        ID = hashmaplinear.hashtable[hashcodeimgstart];
        indexhash = hashCode(Images[ID].data);
        if(indexhash == hashcodeimg)
        {
            for(i=0; i<SIZEOFIMAGE; i++)
            {
                if(Images[ID].data.img_arr[i] != img.img_arr[i])
                    break;
            }
            if(i == SIZEOFIMAGE)//if we find it we will return the id and make it equals to available to know that we can enter in it as it empty place
            {
                hashmaplinear.hashtable[hashcodeimgstart] = AVAILABLE;
                return ID;
            }
        }
        hashcodeimgstart++;
        if(hashcodeimgstart == 1000)
            hashcodeimg = 0;
    }
    if(j==1000)
        return 1000;
}
void putchain(int key, image current_image)
{
    int hashcodenum = hashCode(current_image);
    if(hashmapchain.Chain[hashcodenum].Head == NULL)//if head is null which it is a pointer to linked list means it will enter to initizalize it and enter the value on it
    {
        initLinkedList(&hashmapchain.Chain[hashcodenum]);
        addTail(&hashmapchain.Chain[hashcodenum],key);
    }
    else
    {
        addTail(&hashmapchain.Chain[hashcodenum],key);//if they are a linked list then it will enter next to the node with the same hashcode
        collisionschain++;//increase the number of collisions of chain
    }
}
int getchain(image img)
{
    int hashcodeimg = hashCode(img);
    Node *temp = hashmapchain.Chain[hashcodeimg].Head;//pointer to the head of the linked list
    int ID;//ID of the image to check the pixels
    int indexhash;//index of it in the hashtable
    int j;
    int i;
    if(!temp)//if temp b null then means is removed and return 1000
        return 1000;
    ID = temp->Data;//else continue and take the ID of the image
    indexhash = hashCode(Images[ID].data);//and the hashcode of it from the array of images just check
    while(temp != NULL)
    {
        ID = temp->Data;
        indexhash = hashCode(Images[ID].data);
        if(indexhash == hashcodeimg)//just check that every image we increase in the same node with the same hashcode
        {
            for(i=0; i<SIZEOFIMAGE; i++)//check the pixels to know the right image with the same hashcode
            {
                if(Images[ID].data.img_arr[i] != img.img_arr[i])
                    break;
            }
            if(i == SIZEOFIMAGE)
            {
                return ID;
            }
            temp = temp->next;//if not go to the next node till null
        }
        else
        {
            return 1000;
        }

    }
    return 1000;
}
int removeImagechain(image img)
{
    Node *temp;//it's the start of the head of the linked list
    Node *headtemp;
    int hashcodeimg = hashCode(img);
    temp = hashmapchain.Chain[hashcodeimg].Head;
    headtemp = hashmapchain.Chain[hashcodeimg].Head;
    int ID;
    int indexhash;
    int j;
    int i;

    while(temp != NULL)//loop till temp equal null
    {
        ID = temp->Data;
        indexhash = hashCode(Images[ID].data);
        if(indexhash == hashcodeimg)
        {
            for(i=0; i<SIZEOFIMAGE; i++)
            {
                if(Images[ID].data.img_arr[i] != img.img_arr[i])
                    break;
            }
            if(i == SIZEOFIMAGE)//search for it as get but when we find it we have 3 option either its in the middle
            {
                if(hashmapchain.Chain[hashcodeimg].Head != temp)//means in the middle
                {
                    while(headtemp->next != temp)//we will loop from the head to the node before the target
                    {
                        headtemp=headtemp->next;
                    }
                    if(temp->next)//if it's not the last one
                    {
                        headtemp->next=temp->next;//and make the next of the before it the next of the target
                    }
                    else if(!temp->next)//if it's the last one then we will move the tail and make the next of the target
                    {
                        hashmapchain.Chain[hashcodeimg].Tail=headtemp;
                        headtemp->next=temp->next;
                    }
                    free(temp);
                    return ID;
                }
                else//means the node we want remove is the head and will make the head and the tail equal null and free it
                {
                    hashmapchain.Chain[hashcodeimg].Head=NULL;
                    hashmapchain.Chain[hashcodeimg].Tail=NULL;
                    free(temp);
                    return ID;
                }
            }
            temp = temp->next;
        }
        else
        {
            return 1000;
        }

    }
    return 1000;
}
void chain()
{
    int j;
    clock_t begin1 = clock();
    for(j=0; j<MAXSIZEIMAGES; j++)
    {
        putchain(Images[j].Key,Images[j].data);
    }
    clock_t end1 = clock();
    time_spent_chain = (double)(end1 - begin1) / CLOCKS_PER_SEC;
    /*printf("Time chain: %lf\n",time_spent_chain);
    printf("Number of collisions SEPARATE CHAIN: %d\n",collisionschain);*/

}
void proping()
{
    int j;
    clock_t begin2 = clock();
    for(j=0; j<MAXSIZEIMAGES; j++)
    {
        putlinear(Images[j].Key,Images[j].data);
    }
    clock_t end2 = clock();
    time_spent_linear = (double)(end2 - begin2) / CLOCKS_PER_SEC;
    /* printf("Time proping: %lf\n",time_spent_linear);
     printf("Number of collisions LINEAR PROBING: %d\n",collisionschain);*/
}
void menu()
{
    int choosenum;
    printf("1-Separate Chaining: \n");
    printf("2-Linear Probing: \n");
    printf("3-Exit: \n");
    printf("\n\n Enter your choice: ");
    scanf("%d",&choosenum);
    if(choosenum == 1)
    {
        chainmenu();
    }
    else if(choosenum == 2)
    {
        linearmenu();
    }
    else if(choosenum == 3)
        exit(0);
    else
    {
        printf("Please enter a correct number!!!!\n");
        getch();
        system("cls");
        menu();
    }
}
void searchlinear()
{
    int i=0;
    int getID;
    while(i<=numberofimages)
    {
        getID = getlinear(Imagessearch[i].data);
        if(getID == 1000)
            printf("image not found \n");
        else
            printf("Image Found id = %d\n",getID);
        i++;
    }
}
void removelinear()
{
    int i=0;
    int removeID;
    while(i<=numberofimages)
    {
        removeID = removeImagelinear(Imagessearch[i].data);
        if(removeID == 1000)
            printf("image not found \n");
        else
            printf("Image Found id = %d and deleted\n",removeID);
        i++;
    }
}
void searchchain()
{
    int i=0;
    int getID;
    while(i<=numberofimages)
    {
        getID = getchain(Imagessearch[i].data);
        if(getID == 1000)
            printf("image not found \n");
        else
            printf("Image Found id = %d\n",getID);
        i++;

    }
}
void removechain()
{
    int i=0;
    int removeID;
    while(i<=numberofimages)
    {
        removeID = removeImagechain(Imagessearch[i].data);
        if(removeID == 1000)
            printf("image not found \n");
        else
            printf("Image Found id = %d and deleted\n",removeID);
        i++;
    }
}
void chainmenu()
{
    int choosenum;
    system("cls");
    printf("---------------------Separate Chaining--------------\n\n");
    if(chains>0)
    {
        printf("Time chain: %lf\n",time_spent_chain);
        printf("Number of collisions SEPARATE CHAIN: %d\n",collisionschain);
    }
    chainselected++;
    chains++;
    if(chainselected == 1)
    {
        if(chains == 1)
        {
            chain();
        }
        printf("Time chain: %lf\n",time_spent_chain);
        printf("Number of collisions SEPARATE CHAIN: %d\n",collisionschain);
        printf("Enter the name of the file you want to read: ");
        fflush(stdin);
        scanf("%s",nameoffile);
        ReadFile(nameoffile);
    }
    printf("1-Search: \n");
    printf("2-delete: \n");
    printf("3-Exit: \n");
    scanf("%d",&choosenum);
    if(choosenum == 1)
    {
        searchchain();
    }
    else if(choosenum == 2)
    {
        removechain();
    }
    else if(choosenum == 3)
    {
        exit(0);
    }
    else
    {
        printf("Please enter a correct number!!!!\n");
        getch();
        system("cls");
    }
    printf("Enter a key to return to the chain menu!!");
    getch();
    chainmenu();
}
void linearmenu()
{
    int choosenum;
    system("cls");
    printf("---------------------Linear Probing--------------\n\n");
    if(propings>0)
    {
        printf("Time proping: %lf\n",time_spent_linear);
        printf("Number of collisions LINEAR PROBING: %d\n",collisionslinear);
    }
    propings++;
    propingselected++;
    if(propingselected == 1)
    {
        if(propings == 1)
        {
            proping();
        }
        printf("Time proping: %lf\n",time_spent_linear);
        printf("Number of collisions LINEAR PROBING: %d\n",collisionslinear);
        printf("Enter the name of the file you want to read: ");
        fflush(stdin);
        scanf("%s",nameoffile);
        ReadFile(nameoffile);
    }
    printf("1-Search: \n");
    printf("2-delete: \n");
    printf("3-Exit: \n");
    scanf("%d",&choosenum);
    if(choosenum == 1)
    {
        searchlinear();
    }
    else if(choosenum == 2)
    {
        removelinear();
    }
    else if(choosenum == 3)
    {
        exit(0);
    }
    else
    {
        printf("Please enter a correct number!!!!\n");
        getch();
        system("cls");
        linearmenu();
    }
    printf("Enter a key to return to the chain menu!!");
    getch();
    linearmenu();

}
