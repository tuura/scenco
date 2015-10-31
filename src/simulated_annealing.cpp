#ifdef __linux
	#include "simulated_annealing.h"
#else
	#include "D:\Projects\PRGM_WORKCRAFT\inc\simulated_annealing.h"
#endif

/*SIMULATED ANNEALING*/
/*This function tunes the solution by using simulated annealing method.*/
void start_simulated_annealing(int cpog_count, int tot_enc, int bits){
	int i,m,n,tmp,start,it;
	double proba;
	double alpha =0.996;
	double temperature;
	double epsilon = 0.1;
	double delta;
	double weight_current, weight_next;

#ifdef ACT_PERCENTAGE
	float act,total,res,res_back = 101;
	printf("Percentage till complete:\n");
#endif
	if(unfix)	start = 0;
	else	start = 1;

	int cn = 0;
	if(SET){
		for(i=0; i<cpog_count;i++)
			if(custom_perm[i] != -1) cn++;
	}
	if(cn < cpog_count -4)
	for(i=0;i<num_perm;i++){
		temperature = 10.0;
		it = 0;
		
		// DEBUG PRINTING FOR STATISTCS ABOUT SIMULATED ANNEALING
		/*if((fp = fopen("results/simulated_annealing_fun_opt_5.txt","w")) == NULL){
			printf("Error opening the file on simulated annealing function.\n");
			return;
		}
		if((fpi = fopen("results/simulated_annealing_fun_opt_fluct_5.txt","w")) == NULL){
			printf("Error opening the file on simulated annealing function.\n");
			return;
		}*/

		weight_current = compute_weight(cpog_count, bits, i);

		while(temperature > epsilon){
			/*DEBUG PRINTING: CURRENT ITERATIONS*/			
			/*if( (it % 10) == 0){
				if((err = single_boolean_function(bits,cpog_count,0,i)!= 0)){
					printf("Error on getting boolean function using Espresso. Error code: %d\n", err);
					return;
				}
				if((err = single_boolean_function(bits,cpog_count,1,i)!= 0)){
					printf("Error on getting boolean function using Espresso. Error code: %d\n", err);
					return;
				}
				area = single_equations_abc(cpog_count,bits, i);
				fprintf(fp,"%f %f\n", weight_current, area);
				fprintf(fpi,"%d %f\n",num++, weight_current);
				printf("temperature: %f - current function: %f\n", temperature,weight_current);
			}*/
			
			//COMPUTE NEXT SOLUTION
			if(SET){
				do{
					m = (rand() % (cpog_count - start)) + start;
					n = (rand() % (cpog_count - start)) + start;
				}while(m == n || custom_perm[m] != -1 || custom_perm[n] != -1);	
			}else{
				do{
					m = (rand() % (cpog_count - start)) + start;
					n = (rand() % (cpog_count - start)) + start;
				}while(m == n);
			}		
			tmp = perm[i][m];
			perm[i][m] = perm[i][n];
			perm[i][n] = tmp;

			//COMPUTE COST FUNCTION FOR NEXT SOLUTION
			weight_next = compute_weight(cpog_count, bits, i);

			//COMPARE COST FUNCTIONS
			delta = weight_next -weight_current;

			if(delta < 0){
				//KEEP NEXT SOLUTION
				weight_current = weight_next;
			}else{
				proba = (rand() * 1.00) / RAND_MAX;
				//printf("RAND = %f, %d\n",proba,RAND_MAX);

				//KEEP NEXT SOLUTION IF PROB LESS THAN
				//EXP(-DELTA/TEMPERATURE)
				if(proba < exp(-delta/temperature)){
					weight_current = weight_next;
				}else{
					//KEEP PREVIOUS SOLUTION
					tmp = perm[i][m];
					perm[i][m] = perm[i][n];
					perm[i][n] = tmp;
				}

			}
			
			//COOLING PROCESS
			temperature *=alpha;
			it++;
		}
		//fclose(fp);
		//fclose(fpi);

#ifdef ACT_PERCENTAGE	
		/*PRINTING PERCENTAGE ELAPSED TO COMPLETION*/
		act = i;
		total = num_perm;
		res = (int)((act/total) * 100);
		if(res != res_back){
			printf("%d - ", (int)res);
			res_back = res;
		}
		fflush(stdout);
#endif
	}
	
	counter = num_perm;
#ifdef ACT_PERCENTAGE	
	printf("100\n");
#endif
	return;
}
