/*
 *
 *  Richard Wannall - rwannall - G00619209
 *  CS 367 Lab3
 *
 *
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

main(int argc, char *argv[])
{


    char fileName[256], tlb[256], pageTable[256];

    strcpy(tlb,argv[1]);
    strcpy(pageTable,argv[1]);

    strcat(tlb, ".tlb");
    strcat(pageTable, ".pt");


    FILE *file1;
    file1 = fopen(tlb, "r");
    FILE *file2;
    file2 = fopen(pageTable, "r");

    //read file into array
    int tlbArray[8][6];
    int ptArray[64][2];
    int i;
    int j;

    //PUT IN THE TLB
    //this reads it in. boom done
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 6; j++)
        {
        fscanf(file1, "%d", &tlbArray[i][j]);
        }
    }
    //now the array is set up

    //PUT INTO PAGE TABLE
    for (i = 0; i < 64; i++)
    {
        for (j = 0; j < 2; j++)
        {
            fscanf(file2, "%d", &ptArray[i][j]);
        }
    }


    /*
        All of this stuff must be done while, input > 0. Because when input is negative, then it means to exit.

        Has to be do while, because it has to run at least one time
    */
    int VA;
    int PPN, PA;
    do {
        char input[256];
        printf("\n\nEnter the virtual address (-1 to exit): ");
        scanf("%s", input);
        
        VA = atoi(input);

        /*
            We are told to exit if -1 is entered. So I did this way, on piazza it was said to be ok. 
            That the only negative number to be tested on.....
        */
        if (VA < 0)
        {
            break;
        }

        //if it is too big, then just print that it is illegal
        


    /*
        Now is the time to get all of the indexes. There is a tlbIndex, tlbTag and a ptIndex
        Then use those to check inside of the arrays.

        Also, the VPO is needed, that is going to be the exact same as the PPO

        But first, lets get the indices and tag. Also make the masks as well.

    */

        int tlbIndex, tlbTag, ptIndex;
        int VPN;
        int VPO;



        //this needs to keep only the lower right most 9 bits.
        //0x0...000111111111
        //do an & with that, should return only the bottom ones
        int maskVPO = 511; //000001FF
        VPO = VA & maskVPO;
        


        //the VPN mask, should be the exact opposite.
        //Everything more left, than those 9. Even though it can only be
        //15 bits, who cares. Bigger the better.
        //Still & with the VA, in order to get the VPN. 
        //THEN THOSE VPN MUST BE SHIFTED
        int maskVPN = 4294966784; //FFFFFE00
        VPN = VA & maskVPN;

        //now have to shift to the right 9 times for VPN.
        //Gonna make a mask to thawrt any arithimetic shift
        int mask2VPN = 0x0000003F;
        VPN = VPN >> 9;
        VPN = VPN & mask2VPN;


        //The VPN is now solid. That is all that the ptIndex is
        ptIndex = VPN;


        //However, the tlbIndex is the right 3 bits, and the tlbTag 
        //is the 3 left bits
        int maskTI;
        maskTI = 0x00000007;
        tlbIndex = (VPN & maskTI);

        //to get the tag, move the VPN over to the right, 3 times.
        //Then use the same maskTI
        tlbTag = VPN >> 3;
        tlbTag = tlbTag & maskTI;


        /***
            All of the info is had. Now time to check inside of the arrays.
            Start with the TLB.
        */

        //The valid are at [i][0] and [i][3]. If 0 is valid, then check tag. If not then check 3.
        //If both invalid or tags don't match. Then go to the Page Table



        
        //It should check for the size of the VA, and then do if else on all of these others

        if (VA > 32768)
        {
            printf("virtual: 0x%x", VA);
            printf("\nIllegal virtual address");
        }    
        

        //This means that the PPN is at that spot
        else if ((tlbArray[tlbIndex][0] == 1) && (tlbArray[tlbIndex][1] == tlbTag) )
        {
            PPN = tlbArray[tlbIndex][2];
            //printf("\nthe PPN = %d = %x\n\n", PPN, PPN);
            PPN = PPN << 9;
            PA = PPN | VPO;
            printf("virtual: 0x%x", VA);
            printf("\nPhysical Address = 0x%x (%d) - from the TLB", PA,PA);
        }
        else if ((tlbArray[tlbIndex][3] == 1) && (tlbArray[tlbIndex][4] == tlbTag) )
        {
            PPN = tlbArray[tlbIndex][5];
            PPN = PPN << 9;
            PA = PPN | VPO;
            printf("virtual: 0x%x", VA);
            printf("\nPhysical Address = 0x%x (%d) - from the TLB", PA,PA);
 
        }

        

        //IF IT IS NEITHER OF THOSE, THEN KEEP ON GOING TO THE PAGE TABLE
        else if (ptArray[ptIndex][0] == 1)
        {
            PPN = ptArray[ptIndex][1];
            PPN = PPN << 9;
            PA = PPN | VPO;
            printf("virtual: 0x%x", VA);
            printf("\nPhysical Address = 0x%x (%d) - from the page table", PA,PA);
 
        }

        else
        {
            printf("virtual: 0x%x", VA);
            printf("\nPage fault");
        }


    } while (VA > 0);


}   