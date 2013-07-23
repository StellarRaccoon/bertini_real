#include "derivative_systems.hpp"





void parse_names(int *numItems, char ***itemNames, int **itemLines, FILE *IN, char *name, int num_declarations)
/***************************************************************\
 * USAGE: find the lines where items are declared and find names *
 * ARGUMENTS: input file, declaration name, and number of lines  *
 * RETURN VALUES: number of items, names, and line numbers       *
 * NOTES:                                                        *
 \***************************************************************/
{
  int lineNumber = 1, lengthName = strlen(name), strLength = 0, strSize = 1;
  char ch;
  char *str = (char *)bmalloc(strSize * sizeof(char));
	
  // move through the file looking for the items
  while ((ch = fgetc(IN)) != EOF)
  { // add to str
    if (strLength + 1 == strSize)
    { // increase strSize
      strSize *= 2;
      str = (char *)brealloc(str, strSize * sizeof(char));
    }
    str[strLength] = ch;
    strLength++;
		
    if (strLength == lengthName)
    { // compare against name
      str[strLength] = '\0';
      if (strcmp(str, name) == 0)
      { // we have a good line -- add the items
        ch = fgetc(IN); // skip the space
        addItems(numItems, itemNames, itemLines, IN, lineNumber);
      }
    }
		
    if (ch == '\n')
    { // new line - reset data
      lineNumber++;
      strLength = 0;
    }
  }
	
  // clear memory
  free(str);
	
  return;
}

void addItems(int *numItems, char ***itemNames, int **itemLines, FILE *IN, int lineNumber)
/***************************************************************\
 * USAGE: add the items in the current line                      *
 * ARGUMENTS: input file and current line number                 *
 * RETURN VALUES: number of items, names, and line numbers       *
 * NOTES:                                                        *
 \***************************************************************/
{
  int strLength = 0, strSize = 1, cont = 1;
  char ch;
  char *str = (char *)bmalloc(strSize * sizeof(char));
	
  // initialize ch
  ch = fgetc(IN);
  do
  { // read in next character
    if (ch == ';')
    { // end loop
      cont = 0;
    }
    else
    { // we have a new item to add
      (*numItems)++;
      *itemNames = (char **)brealloc(*itemNames, *numItems * sizeof(char *));
      *itemLines = (int *)brealloc(*itemLines, *numItems * sizeof(int));
			
      // read in the name
      do
      { // save the character
        if (strLength + 1 == strSize)
        { // increase strSize
          strSize *= 2;
          str = (char *)brealloc(str, strSize * sizeof(char));
        }
        str[strLength] = ch;
        strLength++;
				
        // read in the next character
        ch = fgetc(IN);
				
      } while (ch != ',' && ch != ';');
			
      // save the information
      str[strLength] = '\0';
      strLength++;
      (*itemNames)[*numItems - 1] = (char *)bmalloc(strLength * sizeof(char));
      strcpy((*itemNames)[*numItems - 1], str);
      (*itemLines)[*numItems - 1] = lineNumber;
			
      // reset strLength
      strLength = 0;
			
      if (ch == ',')
      { // read in the next character
        ch = fgetc(IN);
      }
    }
  } while (cont);  
	
  free(str);
	
  return;
}





std::string just_constants(boost::filesystem::path filename, 
												 int numConstants, char **consts, int *lineConstants)
{
	
	std::ifstream IN(filename.c_str());
	std::stringstream constant_string;
	
	
	int declares, lineNumber = 1, ii;
	
	int cont = 1;
	// copy lines which do not declare items or define constants (keep these as symbolic objects)
  while (cont)
  { // see if this line number declares items
    declares = 0;
    for (ii = 0; ii < numConstants; ii++)
      if (lineNumber == lineConstants[ii])
        declares = 1;
		
		std::string current_line;
		getline(IN,current_line);
		//		std::cout << "curr line: " << current_line << std::endl;
		
    if (declares)
    { // move past this line
			constant_string << current_line << std::endl;
    }
    else
    { // check to see if this defines a constant - line must be of the form '[NAME]=[EXPRESSION];' OR EOF
			
			std::string name;
			
			size_t found=current_line.find('=');
			if (found!=std::string::npos) {
				name = current_line.substr(0,found);
			}
			
			declares = 0;
			// compare against constants
			for (ii = 0; ii < numConstants; ii++)
				if (strcmp(name.c_str(), consts[ii]) == 0)
					declares = 1;
			
			if (declares)// print line
				constant_string << current_line << std::endl;
		}
		
    // increment lineNumber
    lineNumber++;
		
    // test for EOF
    if (IN.eof())
      cont = 0;
  }
	
	return constant_string.str();
}




void write_matrix_as_constants(mat_mp M, std::string prefix, FILE *OUT)
{

	
	fprintf(OUT,"constant ");
	for (int ii=0; ii<M->rows; ii++) {
		for (int jj=0; jj<M->cols; jj++) {
			fprintf(OUT,"%s_%d_%d",prefix.c_str(),ii+1,jj+1);
			(ii==M->rows-1 && jj==M->cols-1) ? fprintf(OUT,";\n") : fprintf(OUT,", ");
		}
	}
	
	for (int ii=0; ii<M->rows; ii++) {
		for (int jj=0; jj<M->cols; jj++) {
			fprintf(OUT,"%s_%d_%d = ",prefix.c_str(),ii+1,jj+1);
			
			if (mpfr_number_p(M->entry[ii][jj].r) && mpfr_number_p(M->entry[ii][jj].i))
			{
				mpf_out_str(OUT, 10, 0, M->entry[ii][jj].r);
				fprintf(OUT,"+I*");
//				if (mpfr_sgn(Z->i) >= 0)
//					fprintf(fp, "+I*");
				mpf_out_str(OUT, 10, 0, M->entry[ii][jj].i);
				fprintf(OUT, ";\n");
			}
			else
				fprintf(OUT, "NaN+I*NaN;\n");
		}
	}
	
}


void write_vector_as_constants(vec_mp V, std::string prefix, FILE *OUT)
{
	
	
	fprintf(OUT,"constant ");
	for (int ii=0; ii<V->size; ii++) {
			fprintf(OUT,"%s_%d",prefix.c_str(),ii+1);
			(ii==V->size-1) ? fprintf(OUT,";\n") : fprintf(OUT,", ");
	}
	
	for (int ii=0; ii<V->size; ii++) {
			fprintf(OUT,"%s_%d = ",prefix.c_str(),ii+1);
			
			if (mpfr_number_p(V->coord[ii].r) && mpfr_number_p(V->coord[ii].i))
			{
				mpf_out_str(OUT, 10, 0, V->coord[ii].r);
				fprintf(OUT,"+I*");
				//				if (mpfr_sgn(Z->i) >= 0)
				//					fprintf(fp, "+I*");
				mpf_out_str(OUT, 10, 0, V->coord[ii].i);
				fprintf(OUT, ";\n");
			}
			else
				fprintf(OUT, "NaN+I*NaN;\n");
	}
	
}










