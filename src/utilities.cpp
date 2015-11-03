#ifdef __linux
	#include "utilities.h"
#else
	#include "D:\Projects\PRGM_WORKCRAFT\inc\utilities.h"
#endif

/*STRING COMPARE WITH DON'T CARE SUPPORT*/
/*Following function compare two bit strings, and returns 0 if the strings are equals
or 1 if they are different. It supports don't care conditions as well.*/
int strDCcmp(char* str1,char*str2,int bits){
	int diff = 0,i;
	for(i=0;i<bits;i++){
		if(str1[i] == '0' && str2[i] == '1')
			diff = 1;
		if(str1[i] == '1' && str2[i] == '0')
			diff = 1;
	}

	return diff;
}

/*EVALUATION TRUTH TABLE FUNCTION*/
/*Following function evaluates if boolean function can be pre-set to 0 or 1
without passing trough espresso tool. It speeds up a lot the transformation process
from truth table to boolean functions.*/
int eval_function(char* truth,int n){
	int i;
	boolean one = FALSE,zero = FALSE;

	for(i=0;i<n;i++){
		if(truth[i] == '1') one = TRUE;
		if(truth[i] == '0') zero = TRUE;
	}

	if(one && zero) return 0;
	if(one && !zero) return 1;
	if( (!one && !zero)  || (!one && zero) ) return 2;

	return 0;
}

/*PRINT BINARY FUNCTION*/
/*Following function print the binary representation of an integer number.*/
void print_binary(FILE *fp,int n, int bits){
	int i, *vett,j;
	vett = (int*)calloc(bits, sizeof(int));
	//char *number;

	numb = (char*) malloc(sizeof(char) * MAX_NAME);
	//printf("%d - ",n);
	/*DEBUG PRINTING: int number*/
	//if(fp == stdout)	
	//	fprintf(fp,"(%d) ", n);

	for(i=0;i<bits; i++){
		if(n & 1) vett[i] = 1;
		n >>=1;
	}
	j = 0;
	for(i=bits-1; i>= 0;i--){
		if(fp != NULL)
			fprintf(fp,"%d", vett[i]);
		if(vett[i]) numb[j++] = '1';
		else	numb[j++] = '0';
	}
	numb[bits] = '\0';

	//printf("%s\n",number);

	if (fp != NULL)
		fprintf(fp," ");

	return;
}

/*CONVERSION BINARY -> INT FUNCTION*/
/*Following function simply converts char binary into int decimal.*/
int conv_int(char* string, int index){
	int num = 0,i,val = 1;

	for(i=strlen(string)-1;i>= 0;i--){
		if(string[i] == '1') num += val;
		if(string[i] == '-'){
			DC_custom[index] = TRUE;
		}
		if(string[i] == 'X'){
			DC = TRUE;
		}
		val *=2;
	}

	/*if(cnt != 0)
		*used_bits = strlen(string) - cnt;*/


	return num;
}

/*LOGARITHM2 FUNCTION*/
/*Following function simply computes logarithm base 2 of input parameter.*/
int logarithm2(int n){
	int logval = 0, i = 1,j;

	for(j=0;j<MAX_LOG;j++){
		i *= 2;
		if (n == i)
			logval = -1;
	}

	/*DEBUG PRINTING*/
	//printf("logval: %d", logval);

	while(n){
		logval++;
		n >>= 1;
	}
	return logval;
}

/*TRY SWAP FUNCTION*/
/*Following function swap two encoding in a complete solution and compute the
new weight function, if it's less than previous one, the new encodings is kept.*/
boolean try_swap_fun(int ic,int jc,int cpog_count,int bits){
	int tmp;
	float wg = 0,wg_swapped = 0;

	wg = compute_weight(cpog_count, bits, counter);

	tmp = perm[counter][ic];
	perm[counter][ic] = perm[counter][jc];
	perm[counter][jc] = tmp;
	
	wg_swapped = compute_weight(cpog_count, bits, counter);

	if(wg <= wg_swapped){
		tmp = perm[counter][ic];
		perm[counter][ic] = perm[counter][jc];
		perm[counter][jc] = tmp;
		return FALSE;
	}
	return TRUE;
}

double compute_weight(int cpog_count,int bits,int index){
	int i,j,ones;
	double wg = 0;

	for(i=0;i<cpog_count-1;i++){
		for(j=i+1;j<cpog_count;j++){
			//COMPUTE HAMMING DISTANCE
			ones = compute_HD(perm[index][i],i, perm[index][j],j,bits,cpog_count);
			wg += weight_function(opt_diff[i][j],ones);
		}
	}

	return wg;

}

int compute_HD(int n1,int i, int n2,int j,int bits,int cpog_count){
	int ones,bit_diff,q;
	char *number;

	ones = 0;
	if(SET){
		if(!DC_custom[i] && !DC_custom[j]){
			bit_diff = n1 ^ n2;
			for(q = 0; q<bits; q++){
				if(bit_diff & 1) ones++;
				bit_diff >>= 1;
			}
			return ones;
		}
		if(DC_custom[i] && !DC_custom[j]){
			print_binary(NULL,n2, bits);
			number = numb;
			ones = 0;
			char str[MAX_NAME];						
			int_to_string_DC(bits, i, n1, str);
			for(q = 0; q<bits; q++){
				if(str[q] == '0' && number[q] == '1')
					ones++;
				if(str[q] == '1' && number[q] == '0')
					ones++;
			}
			return ones;
		}
		if(!DC_custom[i] && DC_custom[j]){
			print_binary(NULL,n1, bits);
			number = numb;
			char str[MAX_NAME];							
			int_to_string_DC(bits, j, n2, str);
			for(q = 0; q<bits; q++){
				if(str[q] == '0' && number[q] == '1')
					ones++;
				if(str[q] == '1' && number[q] == '0')
					ones++;
			}
			return ones;
		}
		if(DC_custom[i] && DC_custom[j]){
			char str[MAX_NAME], str2[MAX_NAME];
			int_to_string_DC(bits, i, n1, str);					
			int_to_string_DC(bits, j, n2, str2);
			for(q = 0; q<bits; q++){
				if(str[q] == '0' && str2[q] == '1')
					ones++;
				if(str[q] == '1' && str2[q] == '0')
					ones++;
			}
			return ones;
		}
		
			
	}
	bit_diff = n1 ^ n2;
	for(q = 0; q<bits; q++){
		if(bit_diff & 1) ones++;
		bit_diff >>= 1;
	}
	return ones;
}

int check_correctness(int cpog_count, int tot_enc, int bits){

	int result = 0, *opcodes,i,k, it = 0, limit, res_back;
	opcodes = (int*) calloc(tot_enc, sizeof(int));
	char *number;
	boolean ins = FALSE;

	// INSERTING PREDEFINED OP-CODES
	for(i=0;i<cpog_count;i++){
		if(custom_perm[i] != -1 && !DC_custom[i]){
			result++;
		}
	}

	res_back = result;
	limit = 100000;
	while(it < limit){

		// BRING BACK RESULT TO INITIAL STATE
		result = res_back;
		ins = TRUE;

		for(i=0; i<cpog_count; i++){
			if(custom_perm[i] != -1){
				strcpy(manual_file[i],manual_file_back[i]);
				//printf("%s\n", manual_file[i]);
				for(k=0; k<bits; k++){
					// SUBSTITUTE DON'T CARES WITH VALUES
					if(manual_file[i][k] == 'X')
						manual_file[i][k] = (rand() % 2) ? '1': '0';
					// SUBSTITUTE NOT BITS WITH DON'T CARES
					if(manual_file[i][k] == '-')
						manual_file[i][k] = 'X';
				}
				custom_perm[i] = conv_int(manual_file[i], i);
			}
		}

		// SET OP-CODES ALL AVAILABLES
		for(i=0; i<tot_enc; i++)
			opcodes[i] = 0;

		// INSERTING OP-CODE ALREADY FIXED
		for(i=0; i<cpog_count; i++){
			if(custom_perm[i] != -1 && !DC_custom[i]){
				if(opcodes[custom_perm[i]] == 1){
					ins = FALSE;
					if(it > 1000){
						printf(".error \n");
						printf("Op-code %s present multiple times.\n", manual_file_back[i]);
						printf(".end_error \n");
						return 2;
					}
				}
				opcodes[custom_perm[i]] = 1;
				
			}
		}

		// INSERTING OP-CODE WITH DON'T CARES
		for(i=0; i<cpog_count; i++){
			if(custom_perm[i] != -1 && DC_custom[i]){
				result++;
				for(k=0; k<tot_enc; k++){
					print_binary(NULL,k, bits);
					number = numb;
					if(!strDCcmp(number,manual_file[i],bits)){
						if(opcodes[k] == 1){
							ins = FALSE;
							if(it > 1000){
								printf(".error \n");
								print_binary(NULL,k, bits);
								number = numb;
								printf("Op-code %s and %s cannot be set at the same time.\n", manual_file_back[i], number);
								printf(".end_error \n");
								return 2;
							}
						}
						opcodes[k] = 1;
					}
				}
			}

		}

		// VERIFYING ALL THE GRAPHS COULD BE ENCODED
		for(i=0;i<tot_enc && result < cpog_count;i++){
			if(opcodes[i] == 0){
				opcodes[i] = 1;
				result++;
			}
		}

		if(result != cpog_count && it > 1000){
			printf(".error \n");
			printf("Not enough op-codes for all partial order graphs.\n");
			printf(".end_error \n");
			return 2;
		}

		// IF ALL GRAPHS ARE HAS BEEN ENCODED EXIT
		if(result == cpog_count && ins){
			for(i=0; i<cpog_count; i++){
				//printf("%s\n",manual_file[i]);
				custom_perm[i] = custom_perm_back[i];
				strcpy(manual_file[i],manual_file_back[i]);
			}
			return 0;
		}
		it++;
	}
	
	return 1;

}

void int_to_string_DC(int bits, int index, int val, char *str){
	char *vett;
	int k=0,i;
	vett = (char*)malloc(bits * sizeof(char));

	for(i=0;i<bits; i++){
		if(val & 1) vett[i] = '1';
		else vett[i] = '0';
		val >>=1;
	}
	for(i= bits-1; i>= 0; i--){
			str[k++] = vett[i];
	}
	str[k] = '\0';

	for(i=0;i<bits; i++)
		if(manual_file[index][i] == '-')
			str[i] = '-';
	return;
}

/*FILTER FUNCTION*/
/*This function filter the encodings generated with the Exhaustive search approach
with the custom encoding set by the user. Finally in the perm array will be present 
just the encodings with are not filtered by this function.*/

int filter_encodings(int n_cpog, int bits, int tot_enc){
	int i = 0;
	int j = 0;
	int k = 0;
	int index_filter = 0;
	int present = 0, *opcodes;
	char *number;
	boolean out = FALSE;

	opcodes = (int*) calloc(tot_enc, sizeof(int));

	// LOOP OVER EACH ENCODING GENERATED WITH THE EXHAUSTIVE SEARCH
	for(i = 0; i < num_perm;  i++) {
		
		// INITIALISING SUPPORT VARIABLES
		// drop out the encoding, if it does not fit the customisation
		out = FALSE;
		// opcodes already used for DC conditions
		for(j=0; j<tot_enc; j++)
			opcodes[j] = 0;
		// custom opcodes
		for(j=0; j<n_cpog; j++){
			strcpy(manual_file[j],manual_file_back[j]);
			opcodes[perm[i][j]] = 1; //opcodes used by the encoding
		}

		// CHECK FOR FIXED BITS
		for (j=0; j<n_cpog && out == FALSE; j++){

			// OPCODE FIXED
			if(custom_perm[j] != -1){
				print_binary(NULL,perm[i][j], bits);
				number = numb;
				if( strDCcmp(number, manual_file[j], bits) ){
					out = TRUE;
				}
			}
		}

		// CHECK FOR RESERVED OPCODES
		for (j=0; j<n_cpog && out == FALSE; j++){

			present = 0;
		
	
			// OPCODE CONTAINS AT LEAST ONE RESERVED BIT
			if (DC_custom[j] == TRUE &&  out == FALSE){

				// CONVERT OPCODE INT INTO A STRING
				print_binary(NULL,perm[i][j], bits);
				number = numb;

			/*printf("\nDEBUG PRINTING:----------------------------------\n");
			printf("bits: %d, number: %s\n", bits, number);
			printf("DEBUG PRINTING:----------------------------------\n\n");*/

				// SUBSTITUTE THE X BIT OF THE OPCODE WITH THE REAL VALUES GENERATED
				for(k=0; k<bits; k++){
					// SUBSTITUTE DON'T CARES (X) WITH VALUES
					if(manual_file[j][k] == 'X')
						manual_file[j][k] = number[k];
					// SUBSTITUTE RESERVED BITS (-) WITH DON'T CARES (X)
					if(manual_file[j][k] == '-')
						manual_file[j][k] = 'X';
				}

				// CHECK THAT EACH OPCODE WITH DON'T CARE REPRESENTS ONE AND ONLY ONE PO
				for(k=0; k<tot_enc;k++){
					print_binary(NULL,k, bits);
					number = numb;

					if( !strDCcmp(number, manual_file[j], bits) && opcodes[k] == 1){
						present++;
						
						/*printf("\nDEBUG PRINTING:----------------------------------\n");
						printf("bits: %d, number: %s, manual_file[j]: %s\n", bits, number, manual_file[j]);
						printf("DEBUG PRINTING:----------------------------------\n\n");
						fflush(stdout);*/
					}
				}

				(present > 1) ? out =  TRUE : FALSE;
			}
		}

		// IF ENCODING FITS THE CUSTOMISATION INSERT INTO SET OF SOLUTIONS
		if(out == FALSE){
			for(k=0; k<n_cpog; k++){
				perm[index_filter][k] = perm[i][k];
			}
			index_filter++;
		}
	}

	num_perm = index_filter;
	counter = index_filter;

	return 0;
}


/*It concatenates two strings creating the right portion in the memory.*/
char* catMem(char *str1, char *str2){

	char *newStr;

	newStr = (char *) malloc(sizeof(char) * (strlen(str1) + strlen(str2) + 1));
	sprintf(newStr, "%s%s", str1, str2);
	
	free(str1);

	return newStr;
}

/*It concatenates one string with a character creating the right portion in 
the memory.*/
char* catChar(char *str1, char c){

	char *newStr;

	newStr = (char *) malloc(sizeof(char) * (strlen(str1) + 2));
	sprintf(newStr, "%s%c", str1, c);
	
	free(str1);

	return newStr;
}
