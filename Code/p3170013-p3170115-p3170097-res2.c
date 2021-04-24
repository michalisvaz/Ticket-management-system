#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "p3170013-p3170115-p3170097-res2.h"
#include <unistd.h>

pthread_mutex_t lock_telefoners, lock_cashiers, lock_bank_account, lock_number_of_transfer, lock_wait_time, lock_service_time, lock_planA, lock_planB, lock_planC, lock_screen,lock_seed,lock_c1,lock_c2,lock_c3,lock_c4,lock_seats;
int planA[N_zoneA][N_seat];
int planB[N_zoneB][N_seat];
int planC[N_zoneC][N_seat];
int plan[N_zoneA+N_zoneB+N_zoneC]={0};
int seats=0,trans=0,telefoners=N_tel,cashiers=N_cash,c1=0,c2=0,c3=0,c4=0;
unsigned int seed;
float avg_wait_time=0,avg_service_time=0,bank_account=0;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

int main(int argc,char* argv[]){
	int i,j;
	for(i=0;i<N_seat;i++){
		plan[i]	= 0;
	}
	//if user did not give the correct number of arguments
	if(argc!=3){
		printf("Wrong number of arguments\n");
		return -1;
	}
	
	int N_cust=atoi(argv[1]),err;
	seed=atoi(argv[2]);
	pthread_t *threads=(pthread_t*)malloc(N_cust*sizeof(pthread_t));
	int threadid[N_cust];
	
	//if we can't init one of the mutexes
	int rc=pthread_mutex_init(&lock_telefoners, NULL);
	if (rc) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc=pthread_mutex_init(&lock_cashiers, NULL);
	if (rc) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc=pthread_mutex_init(&lock_bank_account, NULL);
	if (rc) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc=pthread_mutex_init(&lock_number_of_transfer, NULL);
	if (rc) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc=pthread_mutex_init(&lock_wait_time, NULL);
	if (rc) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc=pthread_mutex_init(&lock_service_time, NULL);
	if (rc) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc=pthread_mutex_init(&lock_planA, NULL);
	if (rc) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc=pthread_mutex_init(&lock_planB, NULL);
	if (rc) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc=pthread_mutex_init(&lock_planC, NULL);
	if (rc) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc=pthread_mutex_init(&lock_screen, NULL);
	if (rc) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc=pthread_mutex_init(&lock_seed, NULL);
	if (rc) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc=pthread_mutex_init(&lock_c1, NULL);
	if (rc) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc=pthread_mutex_init(&lock_c2, NULL);
	if (rc) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc=pthread_mutex_init(&lock_c3, NULL);
	if (rc) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc=pthread_mutex_init(&lock_c4, NULL);
	if (rc) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc=pthread_mutex_init(&lock_seats, NULL);
	if (rc) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	
	
    //creating the threads
    i=0;
	while(i<N_cust){
	threadid[i]=i+1;
		err = pthread_create(&(threads[i]), NULL, AwesomeThreadFunction, (void*)&threadid[i]); //func name
		if (err){
			printf("Thread can't be created :[%s]\n", strerror(err));
		}
		i++; 
	}	
	
	//join the threads
	for (i = 0; i < N_cust; i++) {
		rc = pthread_join(threads[i], 0);
	}	
	
	//final output
	//clients are numbered by the order their threads were created. It is not the number of their transaction
		for(i=0;i<N_seat*N_zoneA;i++){
			if(!(*(*planA+i))){
				printf("Zone A / Seat %d / EMPTY\n",i+1);
			}else{
				printf("Zone A / Seat %d / Client %d\n",i+1,*(*planA+i));
			}
		}
		for(i=0;i<N_seat*N_zoneB;i++){
			if(!(*(*planB+i))){
				printf("Zone B / Seat %d / EMPTY\n",i+1);
			}else{
				printf("Zone B / Seat %d / Client %d\n",i+1,*(*planB+i));
			}
		}
		for(i=0;i<N_seat*N_zoneC;i++){
			if(!(*(*planC+i))){
				printf("Zone C / Seat %d / EMPTY\n",i+1);
			}else{
				printf("Zone C / Seat %d / Client %d\n",i+1,*(*planC+i));
			}
		}
		printf("Total revenue:\t%.2f\n",bank_account);
		printf("Percentage of calls ended because theater is full:\t%.2f\n",(float)c1/(float)N_cust);
		printf("Percentage of calls ended because there were not enough consecutive seats:\t%.2f\n",(float)c2/(float)N_cust);
		printf("Percentage of calls resulted in a successful booking:\t%.2f\n",(float)c3/(float)N_cust);
		printf("Percentage of calls resulted in credit card payment failure:\t%.2f\n",(float)c4/(float)N_cust);
		printf("Average waiting time:\t%.2f\n",(float)avg_wait_time/(float)N_cust);
		printf("Average service time:\t%.2f\n",(float)avg_service_time/(float)N_cust);
		
	
	
	
	//Destroy mutexes
	rc=pthread_mutex_destroy(&lock_telefoners);
	if(rc){
		printf("ERROR: return code from pthread_mutex_destroy() for mutex lock_telefoners is %d\n", rc);
		return -1;
	}
	rc=pthread_mutex_destroy(&lock_cashiers);
	if(rc){
		printf("ERROR: return code from pthread_mutex_destroy() for mutex lock_cashiers is %d\n", rc);
		return -1;
	}
	rc=pthread_mutex_destroy(&lock_bank_account);
	if(rc){
		printf("ERROR: return code from pthread_mutex_destroy() for mutex lock_bank_account is %d\n", rc);
		return -1;
	}
	rc=pthread_mutex_destroy(&lock_number_of_transfer);
	if(rc){
		printf("ERROR: return code from pthread_mutex_destroy() for mutex lock_number_of_transfer is %d\n", rc);
		return -1;
	}
	rc=pthread_mutex_destroy(&lock_wait_time);
	if(rc){
		printf("ERROR: return code from pthread_mutex_destroy() for mutex lock_wait_time is %d\n", rc);
		return -1;
	}
	rc=pthread_mutex_destroy(&lock_service_time);
	if(rc){
		printf("ERROR: return code from pthread_mutex_destroy() for mutex lock_service_time is %d\n", rc);
		return -1;
	}
	rc=pthread_mutex_destroy(&lock_planA);
	if(rc){
		printf("ERROR: return code from pthread_mutex_destroy() for mutex lock_planA is %d\n", rc);
		return -1;
	}
	rc=pthread_mutex_destroy(&lock_planB);
	if(rc){
		printf("ERROR: return code from pthread_mutex_destroy() for mutex lock_planB is %d\n", rc);
		return -1;
	}
	rc=pthread_mutex_destroy(&lock_planC);
	if(rc){
		printf("ERROR: return code from pthread_mutex_destroy() for mutex lock_planC is %d\n", rc);
		return -1;
	}
	rc=pthread_mutex_destroy(&lock_screen);
	if(rc){
		printf("ERROR: return code from pthread_mutex_destroy() for mutex lock_screen is %d\n", rc);
		return -1;
	}
	rc=pthread_cond_destroy(&cond);
	if(rc){
		printf("ERROR: return code from pthread_cond_destroy() is %d\n", rc);
		return -1;
	}
	rc=pthread_cond_destroy(&cond2);
	if(rc){
		printf("ERROR: return code from pthread_cond2_destroy() is %d\n", rc);
		return -1;
	}
	rc=pthread_mutex_destroy(&lock_seed);
	if(rc){
		printf("ERROR: return code from pthread_mutex_destroy() for mutex lock_screen is %d\n", rc);
		return -1;
	}
	rc=pthread_mutex_destroy(&lock_c1);
	if(rc){
		printf("ERROR: return code from pthread_mutex_destroy() for mutex lock_c1 is %d\n", rc);
		return -1;
	}
	rc=pthread_mutex_destroy(&lock_c2);
	if(rc){
		printf("ERROR: return code from pthread_mutex_destroy() for mutex lock_c2 is %d\n", rc);
		return -1;
	}
	rc=pthread_mutex_destroy(&lock_c3);
	if(rc){
		printf("ERROR: return code from pthread_mutex_destroy() for mutex lock_c3 is %d\n", rc);
		return -1;
	}
	rc=pthread_mutex_destroy(&lock_c4);
	if(rc){
		printf("ERROR: return code from pthread_mutex_destroy() for mutex lock_c4 is %d\n", rc);
		return -1;
	}
	rc=pthread_mutex_destroy(&lock_seats);
	if(rc){
		printf("ERROR: return code from pthread_mutex_destroy() for mutex lock_seats is %d\n", rc);
		return -1;
	}
	free(threads);
	return 0;
}




void* AwesomeThreadFunction(void* vargc){
	int komple,tid=*(int*)vargc,i,j,cashier_time,temp,f,k;
	float cost;
	struct timespec waitStart, waitEnd,serviceStart,serviceEnd;//with the new definition of service time, waiting starts the same time service starts 
	clock_gettime(CLOCK_REALTIME, &waitStart);
	serviceStart.tv_sec=waitStart.tv_sec;
	serviceStart.tv_nsec=waitStart.tv_nsec;
	/*pthread_mutex_lock(&lock_service_time);
	avg_service_time-=(waitStart.tv_sec+waitStart.tv_nsec/BILLION);
	pthread_mutex_unlock(&lock_service_time);*/
	pthread_mutex_lock(&lock_telefoners);
    while (telefoners == 0) {
        pthread_cond_wait(&cond, &lock_telefoners);
    }
    telefoners--;
    pthread_mutex_unlock(&lock_telefoners);
    clock_gettime(CLOCK_REALTIME, &waitEnd);
    double goat = ( waitEnd.tv_sec - waitStart.tv_sec ) + ( waitEnd.tv_nsec - waitStart.tv_nsec ) / BILLION;
	pthread_mutex_lock(&lock_wait_time);
	avg_wait_time+=goat;
	pthread_mutex_unlock(&lock_wait_time);
	pthread_mutex_lock(&lock_seed);
	int choice=rand_r(&seed);
	seed=choice;
	float ch=(float)choice/(float)RAND_MAX;
	if(ch<P_zoneA){
		choice=0;
	}else if(ch<P_zoneA+P_zoneB){
		choice=1;
	}else{
		choice=2;
	}
	int how_many_seats=rand_r(&seed);
	seed=how_many_seats;
	how_many_seats=how_many_seats%(N_seathigh-N_seatlow+1)+N_seatlow;
	int how_many_seconds=rand_r(&seed);
	seed=how_many_seconds;
	how_many_seconds=how_many_seconds%(t_seathigh-t_seatlow+1)+t_seatlow;
	pthread_mutex_unlock(&lock_seed);
	sleep(how_many_seconds);
	if(seats==N_seat*(N_zoneA+N_zoneB+N_zoneC)){
		pthread_mutex_lock(&lock_screen);
		printf("Reservation cancelled because the theater is full\n");
		pthread_mutex_unlock(&lock_screen);
		pthread_mutex_lock(&lock_c1);
		c1++;
		pthread_mutex_unlock(&lock_c1);
		pthread_mutex_lock(&lock_telefoners);
		telefoners++;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&lock_telefoners);
	}else{
		komple=0;
		if(choice==0){
			pthread_mutex_lock(&lock_planA);
			for(i=0;i<N_zoneA;i++){
				if(plan[i]+how_many_seats<=N_seat){
					f=0;
					for(j=0;j<N_seat;j++){
						if(planA[i][j]==0){
							f++;
						}else{
							f=0;
						}
						if(f==how_many_seats){
							for(k=j;k>j-how_many_seats;k--){
								planA[i][k]=tid;
							}
							komple=1+i;
							pthread_mutex_lock(&lock_seats);
							seats+=how_many_seats;
							plan[i]+=how_many_seats;
							pthread_mutex_unlock(&lock_seats);
							break;
						}			
					}
				}
				if(komple){
					break;
				}
			}
			pthread_mutex_unlock(&lock_planA);
		}else if(choice==1){
			pthread_mutex_lock(&lock_planB);
			for(i=N_zoneA;i<N_zoneA+N_zoneB;i++){
				if(plan[i]+how_many_seats<=N_seat){
					f=0;
					for(j=0;j<N_seat;j++){
						if(planB[i-N_zoneA][j]==0){
							f++;
						}else{
							f=0;
						}
						if(f==how_many_seats){
							for(k=j;k>j-how_many_seats;k--){
								planB[i-N_zoneA][k]=tid;
							}
							komple=1+i;
							pthread_mutex_lock(&lock_seats);
							seats+=how_many_seats;
							plan[i]+=how_many_seats;
							pthread_mutex_unlock(&lock_seats);
							break;
						}			
					}
				}
				if(komple){
					break;
				}
			}
			pthread_mutex_unlock(&lock_planB);
		}else{
			pthread_mutex_lock(&lock_planC);
			for(i=N_zoneA+N_zoneB;i<N_zoneA+N_zoneB+N_zoneC;i++){
				if(plan[i]+how_many_seats<=N_seat){
					f=0;
					for(j=0;j<N_seat;j++){
						if(planC[i-N_zoneA-N_zoneB][j]==0){
							f++;
						}else{
							f=0;
						}
						if(f==how_many_seats){
							for(k=j;k>j-how_many_seats;k--){
								planC[i-N_zoneA-N_zoneB][k]=tid;
							}
							komple=1+i;
							pthread_mutex_lock(&lock_seats);
							seats+=how_many_seats;
							plan[i]+=how_many_seats;
							pthread_mutex_unlock(&lock_seats);
							break;
						}			
					}
				}
				if(komple){
					break;
				}
			}
			pthread_mutex_unlock(&lock_planC);
		}
		if(komple){
			if(choice==0){
				cost=how_many_seats*C_zoneA;
			}else if(choice==1){
				cost=how_many_seats*C_zoneB;
			}else{
				cost=how_many_seats*C_zoneC;
			}
			clock_gettime(CLOCK_REALTIME, &waitStart);
			pthread_mutex_lock(&lock_cashiers);
			while (cashiers == 0) {
				pthread_cond_wait(&cond2, &lock_cashiers);
			}
			cashiers--;
			pthread_mutex_unlock(&lock_cashiers);
			clock_gettime(CLOCK_REALTIME, &waitEnd);
			goat = ( waitEnd.tv_sec - waitStart.tv_sec ) + ( waitEnd.tv_nsec - waitStart.tv_nsec ) / BILLION;
			pthread_mutex_lock(&lock_wait_time);
			avg_wait_time+=goat;
			pthread_mutex_unlock(&lock_wait_time);
			pthread_mutex_lock(&lock_seed);
			int cashier_time=rand_r(&seed);
			seed=cashier_time;
			cashier_time=cashier_time%(t_cashhigh-t_cashlow+1)+t_cashlow;
			sleep(cashier_time);
			temp=rand_r(&seed);
			seed=temp;
			ch=(float)temp/(float)RAND_MAX;
			pthread_mutex_unlock(&lock_seed);
			if(ch<P_cardsuccess){
				pthread_mutex_lock(&lock_c3);
				c3++;
				pthread_mutex_unlock(&lock_c3);
				pthread_mutex_lock(&lock_bank_account);
				bank_account+=cost;
				pthread_mutex_unlock(&lock_bank_account);
				pthread_mutex_lock(&lock_screen);
				printf("You booked your seats successfully.\nTotal cost:\t%.2f\nYour seats are: ",cost);
				pthread_mutex_lock(&lock_number_of_transfer);
				trans++;				
				if(choice==0){
					for(j=0;j<N_seat;j++){
						if(planA[i][j]==tid){
							printf("%d,",i*N_seat+j+1);
						}
					}
					printf("\b in zone A.\nYour transaction number is:\t%d\n",trans);
				}else if(choice==1){
					for(j=0;j<N_seat;j++){
						if(planB[i-N_zoneA][j]==tid){
							printf("%d,",(i-N_zoneA)*N_seat+j+1);//there is separate arithmisi for each zone
						}
					}
					printf("\b in zone B.\nYour transaction number is:\t%d\n",trans);
				}else{
					for(j=0;j<N_seat;j++){
						if(planC[i-N_zoneA-N_zoneB][j]==tid){
							printf("%d,",(i-N_zoneA-N_zoneB)*N_seat+j+1);//there is separate arithmisi for each zone
						}
					}
					printf("\b in zone C.\nYour transaction number is:\t%d\n",trans);
				}
				pthread_mutex_unlock(&lock_number_of_transfer);
				pthread_mutex_unlock(&lock_screen);
				pthread_mutex_lock(&lock_telefoners);
				telefoners++;
				pthread_cond_signal(&cond);
				pthread_mutex_unlock(&lock_telefoners);
			}else{
				pthread_mutex_lock(&lock_c4);
				c4++;
				pthread_mutex_unlock(&lock_c4);
				pthread_mutex_lock(&lock_screen);
				printf("Seats couldn't get booked because of credit card failure.\n");
				pthread_mutex_unlock(&lock_screen);
				pthread_mutex_lock(&lock_seats);
				seats-=how_many_seats;
				plan[i]-=how_many_seats;
				if(choice==0){
					for(j=0;j<N_seat;j++){
						pthread_mutex_lock(&lock_planA);
						if(planA[i][j]==tid){
							planA[i][j]=0;
						}
						pthread_mutex_unlock(&lock_planA);
					}
				}else if(choice==1){
					for(j=0;j<N_seat;j++){
						pthread_mutex_lock(&lock_planB);
						if(planB[i-N_zoneA][j]==tid){
							planB[i-N_zoneA][j]=0;
						}
						pthread_mutex_unlock(&lock_planB);
					}
				}else{
					for(j=0;j<N_seat;j++){
						pthread_mutex_lock(&lock_planC);
						if(planC[i-N_zoneA-N_zoneB][j]==tid){
							planC[i-N_zoneA-N_zoneB][j]=0;
						}
						pthread_mutex_unlock(&lock_planC);
					}
				}
				pthread_mutex_unlock(&lock_seats);
				pthread_mutex_lock(&lock_telefoners);
				telefoners++;
				pthread_cond_signal(&cond);
				pthread_mutex_unlock(&lock_telefoners);
			}
		}else{
			pthread_mutex_lock(&lock_screen);
			printf("Reservation cancelled because there weren't enough consecutive seats available\n");
			pthread_mutex_unlock(&lock_screen);
			pthread_mutex_lock(&lock_c2);
			c2++;
			pthread_mutex_unlock(&lock_c2);
			pthread_mutex_lock(&lock_telefoners);
			telefoners++;
			pthread_cond_signal(&cond);
			pthread_mutex_unlock(&lock_telefoners);
		}
	}
	//possibly nothing	
	pthread_mutex_lock(&lock_service_time);
	clock_gettime(CLOCK_REALTIME, &serviceEnd);
	goat = ( serviceEnd.tv_sec - serviceStart.tv_sec ) + ( serviceEnd.tv_nsec - serviceStart.tv_nsec ) / BILLION;//service starts the same time as waiting with the new definition of service time
	avg_service_time+=goat;
	pthread_mutex_unlock(&lock_service_time);
	pthread_mutex_lock(&lock_cashiers);
	cashiers++;
	pthread_cond_signal(&cond2);
	pthread_mutex_unlock(&lock_cashiers);
	pthread_exit(0);
}



