#define MGET(mat, i, j) ((mat)->data[((i)*((mat)->cols)) + (j)])
#define MSET(mat, i, j, x) ((mat)->data[((i)*((mat)->cols)) + (j)] = (x))

#define RESULTS_LEN 5

// Struct to hold a matrix which has been "squished" to one dimension
typedef struct {
    long rows;
    long cols;
    int *data;
} matrix_t;

// This function will compute five different sums of elements in the array. Each
// sum is computed from the set of elements with the same remainder when their
// respective indexes are divided by 5. That is, results[x] is the sum of all
// matrix elements with an index i such that i % 5 == x.
// Arguments:
//    matrix: Pointer to a matrix_t containing data to sum
//    results: Array of 5 longs
void get_every_fifth(const matrix_t *matrix, long results[RESULTS_LEN]);

// A version of get_every_fifth to be written by YOU. Takes the same arguments
// and should provide the same functionality, but in a way which is better
// optimized.
void get_every_fifth_student(const matrix_t *matrix, long* results);

// Takes in a matrix and its dimensions and returns the sum of every element in
// the matrix.
// Arguments
//     matrix: Pointer to a matrix_t containing data to sum
long get_every(const matrix_t *matrix);

// Version of get_every to be written by the YOU. Takes the same arguments and
// should provide the same functionality, but in a way which is better
// optimized.
long get_every_student(const matrix_t *matrix);

// Takes in a matrix and its dimensions and fills the matrix with pseudorandom
// numbers. Will fill the matrix with the same numbers every time.
void set_up_matrix(matrix_t *matrix, int rows, int cols);

// Takes in a matrix and releases any memory allocated for its storage
void free_matrix(matrix_t *matrix);

// Initializes all values of the results array to zero.
void initialize_results(long results[RESULTS_LEN]);
