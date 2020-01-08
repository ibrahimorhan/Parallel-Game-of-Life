//Halil İbrahim Orhan
//2016400054
//Compiling
//Working
#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;
int leftTop(int t,int k,int curIdPs,int sendArray[][360],int recTopRow[]){
	if(k==0){
		return 0;
	}else if(t==0){
		if(curIdPs==1){
			return 0;
		}else{
			if(recTopRow[k-1]==1){
				return 1;
			}
			return 0;
		}
	}else{
		if(sendArray[t-1][k-1]==1){
			return 1;
		}
		return 0;
	}
}
int top(int t,int k,int curIdPs,int sendArray[][360],int recTopRow[]){
	if(t==0){
		if(curIdPs==1){
			return 0;
		}else{
			if(recTopRow[k]==1){
				return 1;
			}
			return 0;
		}
	}else{
		if(sendArray[t-1][k]==1){
			return 1;
		}
		return 0;
	}
}
int rightTop(int t,int k,int curIdPs,int sendArray[][360],int recTopRow[]){
	if(k==359){
		return 0;
	}else if(t==0){
		if(curIdPs==1){
			return 0;
		}else{
			if(recTopRow[k+1]==1){
				return 1;
			}
			return 0;
		}
	}else{
		if(sendArray[t-1][k+1]==1){
			return 1;
		}
		return 0;
	}
	
}
int left(int t,int k,int curIdPs,int sendArray[][360],int recTopRow[]){
	if(k==0){
		return 0;
	}else{
		if(sendArray[t][k-1]==1){
			return 1;
		}
		return 0;
	}
}
int right(int t,int k,int curIdPs,int sendArray[][360],int recTopRow[]){
	if(k==359){
		return 0;
	}else{
		if(sendArray[t][k+1]==1){
			return 1;
		}
		return 0;
	}
}
int botLeft(int t,int k,int curIdPs,int sendArray[][360],int recBotRow[],int rowNum,int totNumPs){
	if(k==0){
		return 0;
	}else if(t==rowNum-1){
		if(curIdPs+1==totNumPs){
			return 0;
		}else{
			if(recBotRow[k-1]==1){
				return 1;
			}
			return 0;
		}
	}else{
		if(sendArray[t+1][k-1]==1){
			return 1;
		}
		return 0;
	}
}
int bot(int t,int k,int curIdPs,int sendArray[][360],int recBotRow[],int rowNum,int totNumPs){
	if(t==rowNum-1){
		if(curIdPs+1==totNumPs){
			return 0;
		}else{
			if(recBotRow[k]==1){
				return 1;
			}
			return 0;
		}
	}else{
		if(sendArray[t+1][k]==1){
			return 1;
		}
		return 0;
	}
}
int botRight(int t,int k,int curIdPs,int sendArray[][360],int recBotRow[],int rowNum,int totNumPs){
	if(k==359){
		return 0;
	}else if(t==rowNum-1){
		if(curIdPs+1==totNumPs){
			return 0;
		}else{
			if(recBotRow[k+1]==1){
				return 1;
			}
			return 0;
		}
	}else{
		if(sendArray[t+1][k+1]==1){
			return 1;
		}
		return 0;
	}
}
int main(int argc, char*argv[])
{
	int iteration=atoi(argv[3]);
	MPI_Init(NULL,NULL);
	int curIdPs;//id of current process
	MPI_Comm_rank(MPI_COMM_WORLD,&curIdPs);
	int totNumPs;//total number of process
	MPI_Comm_size(MPI_COMM_WORLD,&totNumPs);
	if(curIdPs==0){
		string output=argv[2];

		ifstream myfile(argv[1]);
		if(!myfile.is_open()){
			cout<<"file error"<< endl;
		}
		int matrix[360][360]; //input matrix
		int i=0;
		int j=0;
		string line;

		while(getline(myfile,line)){
			stringstream s(line);
			int x=0;
			j=0;
			while(s>>x){
				matrix[i][j]=x;
				j++;

			}
			i++;
		}
		int counter=0;
		int rankNum=1;
		int rowNum=360/(totNumPs-1);
		int sendArray[rowNum][360]; // matrix parts that will be send

		for(int t=0;t<360;t++){
			for(int k=0;k<360;k++){
				sendArray[counter][k]=matrix[t][k];
				
			}

			if(counter+1==rowNum){
				MPI_Send(&rowNum,1,MPI_INT,rankNum,0,MPI_COMM_WORLD);
				MPI_Send(&sendArray,360*rowNum,MPI_INT,rankNum,1,MPI_COMM_WORLD);
				rankNum++;
				counter=0;
			}else{
				counter++;
			}
			
		}

		
		ofstream outfile;
		counter=0;
		outfile.open(output);
		for(int t=1;t<totNumPs;t++){
			int lastArray[rowNum][360];//recieved matrix parts
			MPI_Recv(&lastArray,360*rowNum,MPI_INT,t,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			for(int k=0;k<rowNum;k++){
				for(int l=0;l<360;l++){
					outfile<<lastArray[k][l]<<" ";
				}
				outfile<<endl;
			}
		}
		outfile.close();
		
	}else{
		int sendTopRow[360];
		int sendBotRow[360];
		int recTopRow[360];// recieve the row from process above
		int recBotRow[360];// recieve the row from process below
		int rowNum;

		MPI_Recv(&rowNum,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		int sendArray[rowNum][360];
		MPI_Recv(&sendArray,360*rowNum,MPI_INT,0,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		
		for(int i=0;i<iteration;i++){
			for(int j=0;j<360;j++){
				sendBotRow[j]=sendArray[rowNum-1][j];
				sendTopRow[j]=sendArray[0][j];
			}
			if(curIdPs%2==1){
				if(curIdPs==1){
					MPI_Send(&sendBotRow,360,MPI_INT,2,0,MPI_COMM_WORLD);
					MPI_Recv(&recBotRow,360,MPI_INT,2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
				}else{
					MPI_Send(&sendBotRow,360,MPI_INT,curIdPs+1,0,MPI_COMM_WORLD);
					MPI_Recv(&recTopRow,360,MPI_INT,curIdPs-1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
					MPI_Send(&sendTopRow,360,MPI_INT,curIdPs-1,0,MPI_COMM_WORLD);
					MPI_Recv(&recBotRow,360,MPI_INT,curIdPs+1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
				}
			}else{
				if(curIdPs==totNumPs-1){
					MPI_Recv(&recTopRow,360,MPI_INT,curIdPs-1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
					MPI_Send(&sendTopRow,360,MPI_INT,curIdPs-1,0,MPI_COMM_WORLD);
				}else{
					MPI_Recv(&recTopRow,360,MPI_INT,curIdPs-1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
					MPI_Send(&sendBotRow,360,MPI_INT,curIdPs+1,0,MPI_COMM_WORLD);
					MPI_Recv(&recBotRow,360,MPI_INT,curIdPs+1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
					MPI_Send(&sendTopRow,360,MPI_INT,curIdPs-1,0,MPI_COMM_WORLD);
				}
			}
			
			int tempArray[rowNum][360];
			for(int t=0;t<rowNum;t++){
				for(int k=0;k<360;k++){
					int liveCounter=0;
					liveCounter+=leftTop(t,k,curIdPs,sendArray,recTopRow);
					liveCounter+=top(t,k,curIdPs,sendArray,recTopRow);
					liveCounter+=rightTop(t,k,curIdPs,sendArray,recTopRow);
					liveCounter+=left(t,k,curIdPs,sendArray,recTopRow);
					liveCounter+=right(t,k,curIdPs,sendArray,recTopRow);
					liveCounter+=botLeft(t,k,curIdPs,sendArray,recBotRow,rowNum,totNumPs);
					liveCounter+=bot(t,k,curIdPs,sendArray,recBotRow,rowNum,totNumPs);
					liveCounter+=botRight(t,k,curIdPs,sendArray,recBotRow,rowNum,totNumPs);
					if(liveCounter<2 && (sendArray[t][k]==1)){
						tempArray[t][k]=0;
					}else if(liveCounter>3 && (sendArray[t][k]==1)){
						tempArray[t][k]=0;
					}else if(liveCounter==3 && (sendArray[t][k]==0)){
						tempArray[t][k]=1;
					}else{
						tempArray[t][k]=sendArray[t][k];
					}
					

				}
			}
			for(int t=0;t<rowNum;t++){
				for(int k=0;k<360;k++){
					sendArray[t][k]=tempArray[t][k];
				}
			}
		}
			
		
		MPI_Send(&sendArray,360*rowNum,MPI_INT,0,0,MPI_COMM_WORLD);

	}
	MPI_Finalize();
	
	
	return 0;
}