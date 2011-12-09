/*
SimpleFPS PDDL problem generator v1

DATE   : 20 July 2011
FILE   : SimpleFPS_PDDL_ProblemGenerator.c
AUTHOR : Michail Papakonstantinou
EMAIL  : sdi0600151@di.uoa.gr
WWW    : stavros.lostre.org/sFPS

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXLINE 256
#define MAXFILENAME 256
#define OPEN_WAYPOINT 0.6
#define GUN_CHANCE 0.25
#define KNIFE_CHANCE 0.25
#define FAID_CHANCE 0.25
#define COVER_POINT_CHANCE 1-GUN_CHANCE-KNIFE_CHANCE-FAID_CHANCE

#define NV_CHANCE 0.2
#define DARK 0.9

#define NPC_INJURED 0.7

void copy(char* filename, FILE* fp)
{
	FILE* from=fopen(filename,"r");
	char line[MAXLINE];
	while(!feof(from))
	{
		fgets(line,MAXLINE,from);
		if(feof(from))
			break;
		fputs(line,fp);
	}
	fclose(from);
}

int main(int argc, char** argv)
{
	int goals[4]={1,2,3,4};
	FILE* output;
	char outputFilename[MAXFILENAME];
	int i,j,k;
	int numberOfAreas;
	double connectivityChance,pos;
	int totalPOI;	
	char line[MAXLINE];
	FILE* level;
	FILE* temp;
	int numberOfLevels;
	char filename[MAXFILENAME];
	int playerArea;
	int npcArea;
	int preliferal=0;
	int guns=0;
	int knives=0;
	int meds=0;
	int cover=0;

	srand(time(NULL));

	if(argc<9)
	{
		printf("Sample execution: generator -a <number_of_areas> -c <connectivity_chance>");
            	printf(" -n <total_points_of_interest> -l <levels_to_generate> [optional]-g g1(and/or)g2(and/or)g3(and/or)g4\n");
              	exit(1);
	}

	for(i=1;i<argc;i+=2)
	{
		if(strcmp(argv[i],"-a")==0)
			numberOfAreas=atoi(argv[i+1]);
		else if(strcmp(argv[i],"-c")==0)
			connectivityChance=atof(argv[i+1]);
		else if(strcmp(argv[i],"-n")==0)
			totalPOI=atoi(argv[i+1]);
		else if(strcmp(argv[i],"-l")==0)
			numberOfLevels=atoi(argv[i+1]);
		else if(strcmp(argv[i],"-g")==0)
		{
			for(j=0;j<argc-i && i+j+1!=argc ;j++)
			{
				sprintf(line,"%c",argv[i+j+1][1]);
				goals[j]=atoi(line);
			}
			if(j!=4)
				goals[j]=-1;
			break;
		}
		else
		{
			printf("Sample execution: generator -a <number_of_areas> -c <connectivity_chance>");
			printf(" -n <total_points_of_interest> -l <levels_to_generate>\n");
			exit(2);
		}
	}

	printf("Number of areas are:%d.\n",numberOfAreas);
	printf("Chance areas are connected:%1.2f.\n",connectivityChance);
	printf("Given points of interest to insert:%d.\n",totalPOI);
	printf("Going to create %d levels!\n\n",numberOfLevels);

	for(i=0;i<numberOfLevels;i++)
	{
		temp=fopen("temp","w");

		npcArea=rand()%numberOfAreas;
		playerArea=rand()%numberOfAreas;

		sprintf(filename,"SimpleFPS_Level%d.txt",i);
		level=fopen(filename,"w");

		printf("Creating file:%s..\n",filename);
		printf("For this file npc is going to be at area%d.\n",npcArea);
		printf("While player will be at area%d.\n",playerArea);

		sprintf(line,"; Generated by SimpleFPS problem generator v1 \n");
		sprintf(line+strlen(line),"; AREAS :%d \n",numberOfAreas);
		sprintf(line+strlen(line),"; POINTS OF INTEREST :%d \n\n",totalPOI);
		fputs(line,level);

		sprintf(line,"(define (problem SimpleFPS_PDDL_Level%d)\n",i);
		sprintf(line+strlen(line),"(:domain SimpleFPS_PDDL)\n(:objects\n\t");
		fputs(line,level);
		
		sprintf(line,"p\n\t");
		fputs(line,level);

		for(j=0;j<numberOfAreas;j++)
		{
			sprintf(line,"area%d\n\t",j);
			fputs(line,level);
		}
		sprintf(line,"\n\t");
		fputs(line,level);

		for(j=0;j<numberOfAreas;j++)
		{
			for(k=0;k<j;k++)
			{
				pos=(double)rand()/RAND_MAX;
				if(pos<connectivityChance)
				{
					sprintf(line,"door%d-%d ",j,k);
					fputs(line,level);
					preliferal++;
					sprintf(line,"\n(waypoint door%d-%d)\n",j,k);					
					sprintf(line+strlen(line),"(point-of-interest door%d-%d area%d)\n",j,k,j);
					sprintf(line+strlen(line),"(point-of-interest door%d-%d area%d)\n",j,k,k);
					sprintf(line+strlen(line),"(connected area%d area%d door%d-%d)\n",j,k,j,k);
					sprintf(line+strlen(line),"(connected area%d area%d door%d-%d)\n",k,j,j,k);
					if(pos<OPEN_WAYPOINT)
					{
						sprintf(line+strlen(line),"(open door%d-%d)\n",j,k);
						fputs(line,temp);
					}
					else
					{
						sprintf(line+strlen(line),"(closed door%d-%d)\n",j,k);
						fputs(line,temp);
						sprintf(line,"keycard%d-%d",j,k);
						fputs(line,level);
						sprintf(line,"(point-of-interest keycard%d-%d",j,k);
						sprintf(line+strlen(line)," area%d)\n(item keycard%d-%d)\n",rand()%numberOfAreas,j,k);
						sprintf(line+strlen(line),"(keycard keycard%d-%d door%d-%d)\n",j,k,j,k);
						fputs(line,temp);
					}
					sprintf(line,"\n");
					fputs(line,temp);
					sprintf(line,"\n\t");
					fputs(line,level);
				}
			}
		}
		
		for(j=0;j<totalPOI;j++)
		{
			pos=(double)rand()/RAND_MAX;
			
			if(pos<GUN_CHANCE)
			{
				sprintf(line,"gun%d ",guns);
				fputs(line,level);
				sprintf(line,"\n(point-of-interest gun%d area%d)\n",guns,rand()%numberOfAreas);
				sprintf(line+strlen(line),"(item gun%d)\n",guns);
				sprintf(line+strlen(line),"(gun gun%d)\n",guns);
				pos=(double)rand()/RAND_MAX;
				if(pos<0.4)
				{
					sprintf(line+strlen(line),"(loaded gun%d)\n",guns);
					fputs(line,temp);
				}
				else
				{
					sprintf(line+strlen(line),"(unloaded gun%d)\n",guns);
					sprintf(line+strlen(line),"(point-of-interest ammogun%d area%d)\n",guns,rand()%numberOfAreas);
					sprintf(line+strlen(line),"(item ammogun%d)\n",guns);
					sprintf(line+strlen(line),"(ammo ammogun%d gun%d)\n",guns,guns);
					fputs(line,temp);
					sprintf(line,"ammogun%d \n\t",guns);
					preliferal++;
					fputs(line,level);
				}
				pos=(double)rand()/RAND_MAX;
				if(pos<NV_CHANCE)
				{
					sprintf(line,"(has-nightvision gun%d)\n",guns);
					fputs(line,temp);
				}
				guns++;
			}
			else if(pos<GUN_CHANCE+KNIFE_CHANCE)
			{
				sprintf(line,"knife%d\n\t",knives);
				fputs(line,level);
				sprintf(line,"(point-of-interest knife%d area%d)\n",knives,rand()%numberOfAreas);
				sprintf(line+strlen(line),"(item knife%d)\n(knife knife%d)\n",knives,knives);
				fputs(line,temp);
				knives++;
			}
			else if(pos<GUN_CHANCE+KNIFE_CHANCE+FAID_CHANCE)
			{
				sprintf(line,"firstaid%d\n\t",meds);
				fputs(line,level);
				sprintf(line,"(point-of-interest firstaid%d area%d)\n",meds,rand()%numberOfAreas);
				sprintf(line+strlen(line),"(item firstaid%d)\n(medikit firstaid%d)\n",meds,meds);
				fputs(line,temp);
				meds++;
			}
			else
			{
				sprintf(line,"coverpoint%d\n\t",cover);
				fputs(line,level);
				sprintf(line,"(point-of-interest coverpoint%d area%d)\n",cover,rand()%numberOfAreas);
				sprintf(line+strlen(line),"(cover-point coverpoint%d)\n",cover);
				fputs(line,temp);
				cover++;
			}
		}

		printf("This level will have:\n%d guns.\n%d knives.\n%d medikits.\n%d cover-points.\n",guns,knives,meds,cover);
		printf("%d preliferal items (eg doors,keycards).\n",preliferal); 

		for(j=0;j<numberOfAreas;j++)
		{
			sprintf(line,"control-box%d\n\t",j);
			fputs(line,level);
		}

		sprintf(line,")\n\n");
		fputs(line,level);

		sprintf(line,"(:init\n(npc-unaware)\n(npc-at area%d)\n(npc-not-close-to-point)\n(npc-uncovered)\n",npcArea);
		fputs(line,level);

		pos=(double)rand()/RAND_MAX;
		if(pos<NPC_INJURED)
			sprintf(line,"(npc-injured)\n");
		else
			sprintf(line,"(npc-full-health)\n");
		fputs(line,level);

		sprintf(line,"(player p)\n(point-of-interest p area%d)\n",playerArea,playerArea);
		fputs(line,level);

		for(j=0;j<numberOfAreas;j++)
		{
			pos=(double)rand()/RAND_MAX;
			if(pos<DARK)
			{
				sprintf(line,"(dark area%d)\n",j);
				fputs(line,level);
			}
			else
			{
				sprintf(line,"(lighted area%d)\n",j);
				fputs(line,level);
			}
		}

		for(j=0;j<numberOfAreas;j++)
		{
			sprintf(line,"(point-of-interest control-box%d area%d)\n",j,j);
			fputs(line,level);
			sprintf(line,"(control-box control-box%d)\n",j);
			fputs(line,level);
		}

		fclose(temp);
		temp=fopen("temp","r");
		while(!feof(temp))
		{
			fgets(line,MAXLINE,temp);
			if(feof(temp))
				break;
			fputs(line,level);
		}
		fclose(temp);

		fclose(level);

		for(j=0;j<4 && goals[j]!=-1;j++)
		{
			sprintf(outputFilename,"SimpleFPS_areas-%d_poi-%d_Level%d-g%d.txt",numberOfAreas,totalPOI,i,goals[j]);
			output=fopen(outputFilename,"w");
			copy(filename,output);
			switch(goals[j])
			{
			case 1:
				sprintf(line,"\n)\n\n(:goal (player-wounded) )\n\n)");
				break;
			case 2:
				sprintf(line,"\n)\n\n(:goal (npc-covered) )\n\n)");
				break;
			case 3:
				sprintf(line,"\n)\n\n(:goal (npc-full-health) )\n\n)");
				break;
			case 4:
				sprintf(line,"\n)\n\n(:goal\n\t(and\n\t\t(player-wounded)\n\t\t(npc-covered)\n\t\t(npc-full-health))\n)\n\n)");
				break;
			}
			fputs(line,output);
			fclose(output);
		}

		remove(filename);

		printf("Level%d is complete!\n\n",i);
	
		guns=0;
		knives=0;
		meds=0;
		cover=0;
		preliferal=0;
	}

	remove("temp");

	printf("All done!\n");
	exit(0);
}