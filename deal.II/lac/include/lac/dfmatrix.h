// $Id$

#ifndef __lac_dfmatrix_h
#define __lac_dfmatrix_h

#ifndef __stdio_h
#include <stdio.h>
#endif
#ifndef __lac_dvector_h
#include <lac/dvector.h>
#endif
#ifndef __lac_ivector_h
#include <lac/ivector.h>
#endif

#include <base/exceptions.h>



/**
 *  Double precision full Matrix.
 *  Memory for Components is supplied explicitly <p>
 *  ( ! Amount of memory needs not to comply with actual dimension due to reinitializations ! ) <p>
 *  - all necessary methods for matrices are supplied <p>
 *  - operators available are '=' and '( )' <p>
 *  CONVENTIONS for used 'equations' : <p>
 *  - THIS matrix is always named 'A' <p>
 *  - matrices are always uppercase , vectors and scalars are lowercase <p>
 *  - Transp(A) used for transpose of matrix A
 *
 */
class dFMatrix
{
				     /**
				      * Component-array.
				      */
    double* val;
				     /** 
				      * Dimension. Actual number of Columns
				      */
    unsigned int dim_range;
				     /**
				      * Dimension. Actual number of Rows
				      */
    unsigned int dim_image;
				     /**
				      * Dimension. Determines amount of reserved memory
				      */
    unsigned int val_size;

				     /**
				      * Initialization   . initialize memory for Matrix <p>
				      * ( m rows , n columns )
				      */
    void init (const unsigned int m, const unsigned int n);
    
				     /**
				      *   Access Elements. returns A(i,j)
				      */
    double& el (const unsigned int i, const unsigned int j)  {
      return val[i*dim_range+j];
    };
    
				     /**
				      *   Access Elements. returns A(i,j)
				      */
    double el (const unsigned int i, const unsigned int j) const {
      return val[i*dim_range+j];
    };
    
    
  public:
				     /**@name 1: Basic Object-handling */
				     //@{
				     /**
				      *       Constructor. Dimension = (n,n) <p>
				      *       ->   quadratic matrix (n rows , n columns)
				      */
    dFMatrix (const unsigned int n = 1) { init(n,n); }
    
				     /**
				      *       Constructor. Dimension = (m,n) <p>
				      *       -> rectangular matrix (m rows , n columns)
				      */
    dFMatrix (const unsigned int m,unsigned int n) { init(m,n); }
    
				     /** 
				      * Copy constructor. Be very careful with
				      * this constructor, since it may take a
				      * huge amount of computing time for large
				      * matrices!!
				      */
    dFMatrix (const dFMatrix&);

				     /**
				      *        Destructor. Clears memory
				      */
    ~dFMatrix();
    
				      /**
				      * Comparison operator. Be careful with
				      * this thing, it may eat up huge amounts
				      * of computing time!
				      */
    bool operator == (const dFMatrix &) const;

				     /**
				      *  A = B           . Copy all elements
				      */
    dFMatrix& operator = (const dFMatrix& B);
    
    
				     /**
				      *  U(0-m,0-n) = s  . Fill all elements
				      */
    void fill (const dFMatrix& src,
	       const unsigned int i=0, const unsigned int j=0);
    
				     /**
				      * Change  Dimension.
				      * Set dimension to (m,n) <p>
				      * ( reinit rectangular matrix )
				      */
    void reinit (const unsigned int m, const unsigned int n);
    
				     /**
				      * Change  Dimension.
				      * Set dimension to (n,n) <p>
				      * ( reinit quadratic matrix )
				      */
    void reinit (const unsigned int n) { reinit(n,n); }
    
				     /**
				      * Adjust  Dimension.
				      * Set dimension to ( m(B),n(B) ) <p>
				      * ( adjust to dimensions of another matrix B )
				      */
    void reinit (const dFMatrix& B) { reinit(B.m(), B.n()); }
    
				     /**
				      *  Inquire Dimension (Row) . returns Number of Rows
				      */
    unsigned int m() const { return dim_image; }
    
				     /**
				      *  Inquire Dimension (Col) . returns Number of Columns
				      */
    unsigned int n () const { return dim_range; }
				     //@}
    
    
				     /**@name 2: Data-Access
				      */
				     //@{
				     /**
				      *   Access Elements. returns element at relative 'address' i <p>
				      *   ( -> access to A(i/n , i mod n) )
				      */
    double el (const unsigned int i) const { return val[i]; }
    
				     /**
				      *   Access Elements. returns A(i,j)
				      */
    double operator() (const unsigned int i, const unsigned int j) const
      {
	Assert (i<dim_image, ExcInvalidIndex (i, dim_image));
	Assert (j<dim_range, ExcInvalidIndex (i, dim_range));
	return el(i,j);
      }

				     /**
				      *   Access Elements. returns A(i,j)
				      */
    double& operator() (const unsigned int i, const unsigned int j)
      {
	Assert (i<dim_image, ExcInvalidIndex (i, dim_image));
	Assert (j<dim_range, ExcInvalidIndex (i, dim_range));
	return el(i,j);
      }

				     /**
				      * Set all entries in the matrix to
				      * zero.
				      */
    void clear ();
				     //@}
    
    
				     /**@name 3: Basic applications on matrices
				      */
				     //@{
				     /**
				      *  A+=B            . Simple addition
				      */
    void add (const double s, const dFMatrix& B);

				     /**
				      * A+=Transp(B).
				      * Simple addition of the transpose of B to this
				      */
    void Tadd (const double s, const dFMatrix& B);
    
				     /**
				      * C=A*B.
				      * Matrix-matrix-multiplication 
				      */
    void mmult (dFMatrix& C, const dFMatrix& B) const;
    
				     /**
				      * C=Transp(A)*B.
				      * Matrix-matrix-multiplication using
				      * transpose of this
				      */
    void Tmmult (dFMatrix& C, const dFMatrix& B) const;
    
				     /**
				      *  w (+)= A*v.
				      *  Matrix-vector-multiplication ; <p>
				      *  ( application of this to a vector v )
				      *  flag adding=true : w=+A*v
				      */
    void vmult (dVector& w, const dVector& v, const bool adding=false) const;
    
				     /**
				      *  w (+)= Transp(A)*v.
				      *  Matrix-vector-multiplication ; <p>
				      *  (application of transpose of this to a vector v)
				      *  flag adding=true : w=+A*v
				      */
    void Tvmult (dVector& w, const dVector& v, const bool adding=false) const;

				     /**
				      * A=Inverse(A). Inversion of this by
				      * Gauss-Jordan-algorithm
				      */
    void gauss_jordan ();

				     /**
                                      * Computes the determinant of a matrix.
                                      * This is only implemented for one two and
                                      * three dimensions, since for higher
                                      * dimensions the numerical work explodes.
                                      * Obviously, the matrix needs to be square
                                      * for this function.
                                      */
    double determinant () const;

				     /**
				      * Assign the inverse of the given
				      * matrix to #*this#. This function is
				      * only implemented (hardcoded) for
				      * square matrices of dimension one,
				      * two and three.
				      */
    void invert (const dFMatrix &M);
				     //@}


				     /**@name 4: Basic applications on Rows or Columns
				      */
				     //@{
				     /**
				      *  A(i,1-n)+=s*A(j,1-n).
				      * Simple addition of rows of this
				      */
    void add_row (const unsigned int i, const double s, const unsigned int j);

				     /**
				      *  A(i,1-n)+=s*A(j,1-n)+t*A(k,1-n).
				      *  Multiple addition of rows of this
				      */
    void add_row (const unsigned int i,
		  const double s, const unsigned int j,
		  const double t, const unsigned int k);

				     /**
				      *  A(1-n,i)+=s*A(1-n,j).
				      *  Simple addition of columns of this
				      */
    void add_col (const unsigned int i, const double s, const unsigned int j);

				     /**
				      *  A(1-n,i)+=s*A(1-n,j)+t*A(1-n,k).
				      *  Multiple addition of columns of this
				      */
    void add_col (const unsigned int i,
		  const double s, const unsigned int j,
		  const double t, const unsigned int k);

				     /**
				      * Swap  A(i,1-n) <-> A(j,1-n).
				      * Swap rows i and j of this
				      */
    void swap_row (const unsigned int i, const unsigned int j);

				     /**
				      *  Swap  A(1-n,i) <-> A(1-n,j).
				      *  Swap columns i and j of this
				      */
    void swap_col (const unsigned int i, const unsigned int j);
				     //@}


				     /**@name 5: Mixed stuff. Including more
				      *  applications on matrices
				      */
				     //@{
				     /**
				      *  w=b-A*v.
				      *  Residual calculation , returns |w|
				      */
    double residual (dVector& w, const dVector& v, const dVector& b) const;

				     /**
				      *  Inversion of lower triangle .
				      */
    void forward (dVector& dst, const dVector& src) const;

				     /**
				      *  Inversion of upper triangle .
				      */
    void backward (dVector& dst, const dVector& src) const;

				     /**
				      * QR - factorization of a matrix.
				      * The orthogonal transformation Q is
				      * applied to the vector y and this matrix. <p>
				      * After execution of householder, the upper
				      *  triangle contains the resulting matrix R, <p>
				      * the lower the incomplete factorization matrices.
				      */
    void householder (dVector& y);

				     /**
				      * Least - Squares - Approximation by QR-factorization.
				      */
    double least_squares (dVector& dst, dVector& src);

				     /**
				      *  A(i,i)+=B(i,1-n). Addition of complete
				      *  rows of B to diagonal-elements of this ; <p>
				      *  ( i = 1 ... m )
				      */
    void add_diag (const double s, const dFMatrix& B);

				     /**
				      *  A(i,i)+=s  i=1-m.
				      * Add constant to diagonal elements of this
				      */
    void diagadd (const double& src);

				     /**
				      *  w+=part(A)*v. Conditional partial
				      *  Matrix-vector-multiplication <p>
				      *  (used elements of v determined by x)
				      */
    void gsmult (dVector& w, const dVector& v, const iVector& x) const;


				     /**
				      * Output of the matrix in user-defined format.
				      */
    void print (FILE* fp, const char* format = 0) const;

				     /**
				      * Print the matrix in the usual format,
				      * i.e. as a matrix and not as a list of
				      * nonzero elements. For better
				      * readability, zero elements
				      * are displayed as empty space.
				      *
				      * Each entry is printed in scientific
				      * format, with one pre-comma digit and
				      * the number of digits given by
				      * #precision# after the comma, with one
				      * space following.
				      * The precision defaults to four, which
				      * suffices for most cases. The precision
				      * and output format are {\it not}
				      * properly reset to the old values
				      * when the function exits.
				      *
				      * You should be aware that this function
				      * may produce {\bf large} amounts of
				      * output if applied to a large matrix!
				      * Be careful with it.
				      */
    void print_formatted (ostream &out,
			  const unsigned int presicion=3) const;
				     //@}

				     /**
				      * Exception
				      */
    DeclException2 (ExcInvalidIndex,
		    int, int,
		    << "The given index " << arg1
		    << " should be less than " << arg2 << ".");
				     /**
				      * Exception
				      */
    DeclException2 (ExcDimensionMismatch,
		    int, int,
		    << "The two dimensions " << arg1 << " and " << arg2
		    << " do not match here.");
				     /**
				      * Exception
				      */
    DeclException0 (ExcNotQuadratic);
				     /**
				      * Exception
				      */
    DeclException0 (ExcInternalError);
				     /**
				      * Exception
				      */
    DeclException3 (ExcInvalidDestination,
		    int, int, int,
		    << "Target region not in matrix: size in this direction="
		    << arg1 << ", size of new matrix=" << arg2
		    << ", offset=" << arg3);
				     /**
				      * Exception
				      */
    DeclException1 (ExcNotImplemented,
		    int,
		    << "This function is not implemented for the given"
		    << " matrix dimension " << arg1);
};
#endif
