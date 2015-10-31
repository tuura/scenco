#ifdef __linux
	#include "heuristic.h"
#else
	#include "D:\Projects\PRGM_WORKCRAFT\inc\heuristic.h"
#endif

/*AREA FUNCTION SSD*/
/*Following function computes both the maximum and minimum weight for each encoding permutation
following SSD criterion.*/
float area_encodings_ssd(int cpog, int bits, float *max,int tot_enc,int v){
        int i = 0, j = 0, k = 0, ones = 0;
	float wg = 0.0,min = MAX_WEIGHT;

        for(i = 0; i<counter; i++){
                wg = 0.0;
                for(j = 0; j<cpog-1; j++)
                        for(k = j+1; k < cpog; k++){
				//COMPUTE HAMMING DISTANCE
				ones = compute_HD(perm[i][j],j, perm[i][k],k,bits,cpog);
				wg += weight_function(opt_diff[j][k],ones);
			}
                weights[i] = wg;
                if(wg < min) min = wg;
                if(wg > (*max)) (*max) = wg;
        }

        return min;
}

/*FUNCTION TO MINISIME*/
float  weight_function(int Mij,int HDij){
	return ((Mij-HDij) * (Mij - HDij));
}

/*MINIMINUM HAMMING DISTANCE FUNCTION v2*/
/*It returns minimum encoding available among available ones.*/
void HD_min_v2(int *encod,int tot_enc,int *enc1,int *enc2,int bits,int sel,int *sol,int i_min,int j_min,int cpog_count){
	int i,j,ones,bit_diff,l;
	long long int min = MAX_WEIGHT;
	int n = 1,r = 1,k,p,where;
	int vi[MAX_CPOG],vj[MAX_CPOG],vres[MAX_CPOG];
	float wg = 0;
	
	//IF SEL == 2, WE NEED 2 ENCODINGS OWNED BY TWO CPOG
	if(sel == 2){
		
		//LOOK FOR ENCDODING WITH MINIMUM HAMMING DISTANCE
		//AMONG AVAILABLE ONES
		for(i=0;i<tot_enc-1;i++){
			for(j=i+1;j<tot_enc;j++){
				if(encod[i] == 0 && encod[j] == 0){
					bit_diff = i ^ j;
					ones = 0;
					for(l = 0; l<bits; l++){
						if(bit_diff & 1) ones++;
		    				bit_diff >>= 1;
					}

					if (ones < min){
						min = ones;
						n = 0;
						vi[n] = i;
						vj[n] = j;
						n++;	
					}else if(ones == min){
						vi[n] = i;
						vj[n] = j;
						n++;
					}
				}
			}
		}
		//IF JUST A COUPLE OF ENCODING
		//RETURN THAT RESULT
		if(n == 1){
			encod[vi[0]] = 1;
			encod[vj[0]] = 1;
			(*enc1) = vi[0];
			(*enc2) = vj[0];
		}
		//OTHERWISE SELECT WHICH
		//COUPLE OF ENCODINGS CONSIDER
		//BY MAXIMISING FUNCTION
		else{
			//CHECK THE ENCODING MINIMISING FUNCTION
			for(i=0;i<n;i++){
				wg = 0;
				min = MAX_WEIGHT;
				sol[i_min] = vi[i];
				sol[j_min] = vj[i];
				for(j=0;j<cpog_count-1;j++){
					for(l=j+1;l<cpog_count;l++){
						if(sol[j] != -1 && sol[l] != -1){
							//COMPUTE HAMMING DISTANCE
							bit_diff = sol[j] ^ sol[l];
							ones = 0;
							for(k = 0; k<bits; k++){
								if(bit_diff & 1) ones++;
				    				bit_diff >>= 1;
							}
							wg += weight_function(opt_diff[j][l],ones);
						}
					}
				}
				if(wg < min){
					min = wg;
					vres[0] = i;
					r = 1;
				}else if (wg == min){
					vres[r++] = i;
				}
			}
			sol[i_min] = -1;
			sol[j_min] = -1;
			l = rand() % r;
			encod[vi[vres[l]]] = 1;
			encod[vj[vres[l]]] = 1;
			(*enc1) = vi[vres[l]];
			(*enc2) = vj[vres[l]];
		}
	}
	//IF SEL == 1, ONE OF TWO CPOG HAS BEEN ALREADY ENCODED
	else{
		//SET THAT ENCODING
		i = (*enc1);

		//AND PICK UP ANOTHER ENCODING MINIMISES
		//HAMMING DISTANCE WITH PREVIOUS ONE
		for(j = 0;j<tot_enc;j++){
			if(j != i && encod[j] == 0){
				bit_diff = i ^ j;
				ones = 0;
				for(l = 0; l<bits; l++){
					if(bit_diff & 1) ones++;
	    				bit_diff >>= 1;
				}

				if (ones < min){
					min = ones;
					n = 0;
					vj[n] = j;
					n++;	
				}else if(ones == min){
					vj[n] = j;
					n++;
				}
			}
		}
		
		//IF JUST ONE ENCODING EXISTS
		//PICK IT UP
		if(n == 1){
			encod[vj[0]] = 1;
			(*enc2) = vj[0];
		}
		//OTHERWISE PICK ENCODING
		//WHICH WHICH MAXIMISE FUNCTION
		else{
			if(sol[i_min] == -1){
				where = 0;
			}
			else{
				where = 1;
			}
			for(k=0;k<n;k++){
				wg = 0;
				min = MAX_WEIGHT;
				if(where == 0)	sol[i_min] = vj[k];
				else	sol[j_min] = vj[k];
				for(j=0;j<cpog_count-1;j++){
					for(l=j+1;l<cpog_count;l++){
						if(sol[j] != -1 && sol[l] != -1){
							//COMPUTE HAMMING DISTANCE
							bit_diff = sol[j] ^ sol[l];
							ones = 0;
							for(p = 0; p<bits; p++){
								if(bit_diff & 1) ones++;
				    				bit_diff >>= 1;
							}
							wg += weight_function(opt_diff[j][l],ones);
						}
					}
				}
				if(wg < min){
					min = wg;
					r = 0;
					vres[r++] = k;
				}else if (wg == min){
					vres[r++] = k;
				}
			}
			if(where == 0)	sol[i_min] = -1;
			else	sol[j_min] = -1;
			p = rand() % r;
			encod[vj[vres[p]]] = 1;
			(*enc2) = vj[vres[p]];
		}
	}

	return;
}


/*MINIMINUM HAMMING DISTANCE FUNCTION v3*/
/*It returns minimum encoding available among available ones.*/
/*v3: try every possible combination minimising funcion*/
void HD_min_v3(int *encod,int tot_enc,int *enc1,int *enc2,int bits,int sel,int *sol,int i_min,int j_min,int cpog_count){
	int i,j,ones,l;
	long long int min = MAX_WEIGHT;
	int r = 1,k,p,where;
	int *vi, *vj;
	float wg = 0;
	
	vi = (int *) malloc(sizeof(int) * (tot_enc * tot_enc));
	vj = (int *) malloc(sizeof(int) * (tot_enc * tot_enc));

	//IF SEL == 2, WE NEED 2 ENCODINGS OWNED BY TWO CPOG
	if(sel == 2){
		
		//LOOK FOR ENCDODING WITH MINIMUM HAMMING DISTANCE
		//AMONG AVAILABLE ONES
		min = MAX_WEIGHT;		
		for(i=0;i<tot_enc-1;i++){
			for(j=i+1;j<tot_enc;j++){
				if(encod[i] == 0 && encod[j] == 0){
					//CHECK THE ENCODING MINIMISING FUNCTION
					wg = 0;
					sol[i_min] = i;
					sol[j_min] = j;
					for(k=0;k<cpog_count-1;k++){
						for(p=k+1;p<cpog_count;p++){
							if(sol[k] != -1 && sol[p] != -1){
								//COMPUTE HAMMING DISTANCE
								ones = compute_HD(sol[k],k,sol[p],p, bits,cpog_count);
								//printf("%d xor %d = %d\n",sol[k],sol[p],ones);
								wg += weight_function(opt_diff[k][p],ones);
							}
						}
					}
					if(wg < min){
						min = wg;
						vi[0] = i;
						vj[0] = j;
						r = 1;
					}else if (wg == min){
						vi[r] = i;
						vj[r++] = j;
					}
				}
			}
		}
		//IF JUST A COUPLE OF ENCODING
		//RETURN THAT RESULT
		if(r == 1){
			encod[vi[0]] = 1;
			encod[vj[0]] = 1;
			(*enc1) = vi[0];
			(*enc2) = vj[0];
		}
		//OTHERWISE SELECT WHICH
		//COUPLE OF ENCODINGS CONSIDER
		//BY MAXIMISING FUNCTION
		else{
			l = rand() % r;
			sol[i_min] = vi[l];
			sol[j_min] = vj[l];
			encod[vi[l]] = 1;
			encod[vj[l]] = 1;
			(*enc1) = vi[l];
			(*enc2) = vj[l];
		}
	}
	//IF SEL == 1, ONE OF TWO CPOG HAS BEEN ALREADY ENCODED
	else{
		//SET THAT ENCODING
		i = (*enc1);
		if(sol[i_min] == -1){
			where = 0;
		}
		else{
			where = 1;
		}
		//AND PICK UP ANOTHER ENCODING MINIMISES
		//HAMMING DISTANCE WITH PREVIOUS ONE
		for(j = 0;j<tot_enc;j++){
			if(j != i && encod[j] == 0){
				//CHECK THE ENCODING MINIMISING FUNCTION
				wg = 0;
				if(where == 0)	sol[i_min] = j;
				else	sol[j_min] = j;
				for(k=0;k<cpog_count-1;k++){
					for(p=j+1;p<cpog_count;p++){
						if(sol[k] != -1 && sol[p] != -1){
							//COMPUTE HAMMING DISTANCE
							ones = compute_HD(sol[k],k,sol[p],p, bits,cpog_count);
							//printf("%d xor %d = %d\n",sol[k],sol[p],ones);
							wg += weight_function(opt_diff[k][p],ones);
						}
					}
				}
				if(wg < min){
					min = wg;
					vj[0] = j;
					r = 1;
				}else if (wg == min){
					vj[r++] = j;
				}
			}
		}
		
		//IF JUST ONE ENCODING EXISTS
		//PICK IT UP
		if(r == 1){
			encod[vj[0]] = 1;
			(*enc2) = vj[0];
		}
		//OTHERWISE PICK ENCODING
		//WHICH WHICH MAXIMISE FUNCTION
		else{
			p = rand() % r;
			if(where == 0)	sol[i_min] = vj[p];
			else	sol[j_min] = vj[p];
			encod[vj[p]] = 1;
			(*enc2) = vj[p];
		}
	}

	free(vi);
	free(vj);

	return;
}

void random_opcode_choice_v2(int *encod,int tot_enc,int *enc1,int *enc2,int bits,int sel,int *sol,int i_min,int j_min,int cpog_count){
	int i,j,l;
	int r,p,where;
	int *vi, *vj;
	
	vi = (int *) malloc(sizeof(int) * (tot_enc * tot_enc));
	vj = (int *) malloc(sizeof(int) * (tot_enc * tot_enc));

	//IF SEL == 2, WE NEED 2 ENCODINGS OWNED BY TWO CPOG
	if(sel == 2){
		
		//LOOK FOR ENCDODING WITH MINIMUM HAMMING DISTANCE
		//AMONG AVAILABLE ONES
		r = 0;
		for(i=0;i<tot_enc-1;i++){
			for(j=i+1;j<tot_enc;j++){
				if(encod[i] == 0 && encod[j] == 0){
					sol[i_min] = i;
					sol[j_min] = j;
					vi[r] = i;
					vj[r++] = j;
				}
			}
		}
		//IF JUST A COUPLE OF ENCODING
		//RETURN THAT RESULT
		if(r == 1){
			encod[vi[0]] = 1;
			encod[vj[0]] = 1;
			(*enc1) = vi[0];
			(*enc2) = vj[0];
		}
		//OTHERWISE SELECT WHICH
		//COUPLE OF ENCODINGS CONSIDER
		//BY MAXIMISING FUNCTION
		else{
			l = rand() % r;
			sol[i_min] = vi[l];
			sol[j_min] = vj[l];
			encod[vi[l]] = 1;
			encod[vj[l]] = 1;
			(*enc1) = vi[l];
			(*enc2) = vj[l];
		}
	}
	//IF SEL == 1, ONE OF TWO CPOG HAS BEEN ALREADY ENCODED
	else{
		//SET THAT ENCODING
		i = (*enc1);
		if(sol[i_min] == -1){
			where = 0;
		}
		else{
			where = 1;
		}
		//AND PICK UP ANOTHER ENCODING MINIMISES
		//HAMMING DISTANCE WITH PREVIOUS ONE
		r = 0; // MOD
		for(j = 0;j<tot_enc;j++){
			if(j != i && encod[j] == 0){
				//CHECK THE ENCODING MINIMISING FUNCTION
				if(where == 0)	sol[i_min] = j;
				else	sol[j_min] = j;
					vj[r++] = j;
			}
		}
		
		//IF JUST ONE ENCODING EXISTS
		//PICK IT UP
		if(r == 1){
			encod[vj[0]] = 1;
			(*enc2) = vj[0];
		}
		//OTHERWISE PICK ENCODING
		//WHICH WHICH MAXIMISE FUNCTION
		else{
			p = rand() % r;
			if(where == 0)	sol[i_min] = vj[p];
			else	sol[j_min] = vj[p];
			encod[vj[p]] = 1;
			(*enc2) = vj[p];
		}
	}

	free(vi);
	free(vj);

	return;
}
