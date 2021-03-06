#include <dplyr.h>
using namespace dplyr ;
using namespace Rcpp ;

namespace dplyr{

    // -------------- (int,lgl)
    template <int LHS_RTYPE, int RHS_RTYPE>
    inline size_t hash_int_int( JoinVisitorImpl<LHS_RTYPE,RHS_RTYPE>& joiner, int i){
        return joiner.RHS_hash_fun( i>=0 ? joiner.left[i] : joiner.right[-i-1] ) ;  
    }
    template <>
    inline size_t JoinVisitorImpl<INTSXP,LGLSXP>::hash( int i){
        return hash_int_int<INTSXP,LGLSXP>( *this, i) ;   
    }
    template <>
    inline size_t JoinVisitorImpl<LGLSXP,INTSXP>::hash( int i){
        return hash_int_int<LGLSXP,INTSXP>( *this, i) ;   
    }
    template <int LHS_RTYPE, int RHS_RTYPE>
    inline SEXP subset_join_int_int( JoinVisitorImpl<LHS_RTYPE,RHS_RTYPE>& joiner, const std::vector<int>& indices ){
        int n = indices.size() ;
        IntegerVector res = no_init(n) ;
        for( int i=0; i<n; i++) {
            int index = indices[i] ;
            if( index >= 0 ){  
                res[i] = joiner.left[index] ;
            } else {
                res[i] = joiner.right[-index-1] ;    
            }
        }
        return res ;    
    }
    template <>
    inline SEXP JoinVisitorImpl<INTSXP,LGLSXP>::subset( const std::vector<int>& indices ){
        return subset_join_int_int<INTSXP,LGLSXP>( *this, indices ) ;
    }
    template <>
    inline SEXP JoinVisitorImpl<LGLSXP,INTSXP>::subset( const std::vector<int>& indices ){
        return subset_join_int_int<LGLSXP,INTSXP>( *this, indices ) ;
    }
    
    template <int LHS_RTYPE, int RHS_RTYPE>
    inline SEXP subset_join_int_int( JoinVisitorImpl<LHS_RTYPE,RHS_RTYPE>& joiner, const VisitorSetIndexSet<DataFrameJoinVisitors>& set ){
        int n = set.size() ;
        IntegerVector res = no_init(n) ;
        VisitorSetIndexSet<DataFrameJoinVisitors>::const_iterator it=set.begin() ;
        for( int i=0; i<n; i++, ++it) {
            int index = *it ;
            if( index >= 0 ){  
                res[i] = joiner.left[index] ;
            } else {
                res[i] = joiner.right[-index-1] ;    
            }
        }
        return res ;    
    }
    template <>
    inline SEXP JoinVisitorImpl<INTSXP,LGLSXP>::subset( const VisitorSetIndexSet<DataFrameJoinVisitors>& set ){
        return subset_join_int_int<INTSXP,LGLSXP>( *this, set ) ;
    }
    template <>
    inline SEXP JoinVisitorImpl<LGLSXP,INTSXP>::subset( const VisitorSetIndexSet<DataFrameJoinVisitors>& set ){
        return subset_join_int_int<LGLSXP,INTSXP>( *this, set ) ;
    }
    
    
    // -------------- (int,double)
    template <int RTYPE>
    inline size_t hash_int_double( JoinVisitorImpl<RTYPE,REALSXP>& joiner, int i ){
        if( i>=0 ){
            int val = joiner.left[i] ;
            if( val == NA_INTEGER ) return joiner.RHS_hash_fun( NA_REAL );
            return joiner.RHS_hash_fun( (double)val );
        }
        return joiner.RHS_hash_fun( joiner.right[-i-1] ) ; 
    }
    template <>
    inline size_t JoinVisitorImpl<INTSXP,REALSXP>::hash(int i){
        return  hash_int_double<INTSXP>( *this, i );  
    }
    template <>
    inline size_t JoinVisitorImpl<LGLSXP,REALSXP>::hash(int i){
        return  hash_int_double<LGLSXP>( *this, i );  
    }
    
    
    template <int RTYPE>
    inline SEXP subset_join_int_double( JoinVisitorImpl<RTYPE,REALSXP>& joiner, const std::vector<int>& indices ){
        int n = indices.size() ;
        NumericVector res = no_init(n) ;
        for( int i=0; i<n; i++) {
            int index = indices[i] ;
            if( index >= 0 ){  
                res[i] = Rcpp::internal::r_coerce<INTSXP,REALSXP>( joiner.left[index] ) ;
            } else {
                res[i] = joiner.right[-index-1] ;    
            }
        }
        return res ;    
    }
    template <>
    inline SEXP JoinVisitorImpl<INTSXP,REALSXP>::subset( const std::vector<int>& indices ){
        return subset_join_int_double<INTSXP>( *this, indices ) ;
    }
    template <>
    inline SEXP JoinVisitorImpl<LGLSXP,REALSXP>::subset( const std::vector<int>& indices ){
        return subset_join_int_double<LGLSXP>( *this, indices ) ;
    }
    
    
    template <int RTYPE>
    inline SEXP subset_join_int_double( JoinVisitorImpl<RTYPE,REALSXP>& joiner, const VisitorSetIndexSet<DataFrameJoinVisitors>& set ){
        int n = set.size() ;
        NumericVector res = no_init(n) ;
        VisitorSetIndexSet<DataFrameJoinVisitors>::const_iterator it=set.begin() ;
        for( int i=0; i<n; i++, ++it) {
            int index = *it ;
            if( index >= 0){
                res[i] = Rcpp::internal::r_coerce<INTSXP,REALSXP>( joiner.left[index] ) ;    
            } else {
                res[i] = joiner.right[-index-1] ;    
            }
        }
        return res ;    
    }
    template <>
    inline SEXP JoinVisitorImpl<INTSXP,REALSXP>::subset( const VisitorSetIndexSet<DataFrameJoinVisitors>& set ){
        return  subset_join_int_double<INTSXP>(*this, set );       
    }
    template <>
    inline SEXP JoinVisitorImpl<LGLSXP,REALSXP>::subset( const VisitorSetIndexSet<DataFrameJoinVisitors>& set ){
        return  subset_join_int_double<LGLSXP>(*this, set );       
    }
    
    // -------------- (double,int)
    template <int RTYPE>
    inline size_t hash_double_int( JoinVisitorImpl<REALSXP,RTYPE>& joiner, int i ){
        if( i<0 ){
            int val = joiner.left[-i-1] ;
            if( val == NA_INTEGER ) return joiner.LHS_hash_fun( NA_REAL );
            return joiner.LHS_hash_fun( (double)val );
        }
        return joiner.LHS_hash_fun( joiner.right[i] ) ; 
    }
    template <>
    inline size_t JoinVisitorImpl<REALSXP,INTSXP>::hash(int i){
        return  hash_double_int<INTSXP>( *this, i );  
    }
    template <>
    inline size_t JoinVisitorImpl<REALSXP,LGLSXP>::hash(int i){
        return  hash_double_int<LGLSXP>( *this, i );  
    }
    
    
    template <int RTYPE>
    inline SEXP subset_join_double_int( JoinVisitorImpl<REALSXP,RTYPE>& joiner, const std::vector<int>& indices ){
        int n = indices.size() ;
        NumericVector res = no_init(n) ;
        for( int i=0; i<n; i++) {
            int index = indices[i] ;
            if( index < 0 ){  
                res[i] = Rcpp::internal::r_coerce<INTSXP,REALSXP>( joiner.right[-index-1] ) ;
            } else {
                res[i] = joiner.left[index] ;    
            }
        }
        return res ;    
    }
    template <>
    inline SEXP JoinVisitorImpl<REALSXP,INTSXP>::subset( const std::vector<int>& indices ){
        return subset_join_double_int<INTSXP>( *this, indices ) ;
    }
    template <>
    inline SEXP JoinVisitorImpl<REALSXP,LGLSXP>::subset( const std::vector<int>& indices ){
        return subset_join_double_int<LGLSXP>( *this, indices ) ;
    }
    
    
    template <int RTYPE>
    inline SEXP subset_join_double_int( JoinVisitorImpl<REALSXP,RTYPE>& joiner, const VisitorSetIndexSet<DataFrameJoinVisitors>& set ){
        int n = set.size() ;
        NumericVector res = no_init(n) ;
        VisitorSetIndexSet<DataFrameJoinVisitors>::const_iterator it=set.begin() ;
        for( int i=0; i<n; i++, ++it) {
            int index = *it ;
            if( index < 0){
                res[i] = Rcpp::internal::r_coerce<INTSXP,REALSXP>( joiner.right[-index-1] ) ;    
            } else {
                res[i] = joiner.left[index] ;    
            }
        }
        return res ;    
    }
    template <>
    inline SEXP JoinVisitorImpl<REALSXP,INTSXP>::subset( const VisitorSetIndexSet<DataFrameJoinVisitors>& set ){
        return  subset_join_double_int<INTSXP>(*this, set );       
    }
    template <>
    inline SEXP JoinVisitorImpl<REALSXP,LGLSXP>::subset( const VisitorSetIndexSet<DataFrameJoinVisitors>& set ){
        return  subset_join_double_int<LGLSXP>(*this, set );       
    }
    
    
    
    // -----------------
    inline void incompatible_join_visitor(SEXP left, SEXP right, const std::string& name) {
        std::stringstream s ;
        s << "Can't join on '" << name << "' because of incompatible types (" << get_single_class(left) << "/" << get_single_class(right) << ")" ;
        stop( s.str() ) ;    
    }
    
    JoinVisitor* join_visitor( SEXP left, SEXP right, const std::string& name){
        switch( TYPEOF(left) ){
            case INTSXP:
                {
                    bool lhs_factor = Rf_inherits( left, "factor" ) ;
                    switch( TYPEOF(right) ){
                        case INTSXP:
                            {
                                bool rhs_factor = Rf_inherits( right, "factor" ) ;
                                if( lhs_factor && rhs_factor){
                                    return new JoinFactorFactorVisitor(left, right) ;
                                } else if( !lhs_factor && !rhs_factor) {
                                    return new JoinVisitorImpl<INTSXP, INTSXP>( left, right ) ;
                                }
                                break ;
                            }
                        case REALSXP:   
                            {
                                if( lhs_factor ){ 
                                    incompatible_join_visitor(left, right, name) ;
                                } else if( is_bare_vector(right) ) {
                                    return new JoinVisitorImpl<INTSXP, REALSXP>( left, right) ;
                                } else {
                                    incompatible_join_visitor(left, right, name) ;
                                }
                                break ;
                                // what else: perhaps we can have INTSXP which is a Date and REALSXP which is a Date too ?
                            }
                        case LGLSXP:  
                            {
                                if( lhs_factor ){
                                    incompatible_join_visitor(left, right, name) ;
                                } else {
                                    return new JoinVisitorImpl<INTSXP, LGLSXP>( left, right) ;    
                                }
                                break ;
                            }
                        case STRSXP:
                            {
                                if( lhs_factor ){
                                    return new JoinFactorStringVisitor( left, right );     
                                }
                            }
                        default: break ;
                    }
                    break ;  
                }
            case REALSXP:
                {    
                    bool lhs_date = Rf_inherits( left, "Date" ) ;
                    bool lhs_time = Rf_inherits( left, "POSIXct" );
                    
                    switch( TYPEOF(right) ){
                    case REALSXP:
                        {
                            if( Rf_inherits( right, "Date") ){
                                if(lhs_date) return new DateJoinVisitor(left, right ) ;
                                incompatible_join_visitor(left, right, name) ;
                            }
                            
                            if( Rf_inherits( right, "POSIXct" ) ){
                                if( lhs_time ) return new POSIXctJoinVisitor(left, right ) ;
                                incompatible_join_visitor(left, right, name) ;
                            }
                            
                            if( is_bare_vector( right ) ){
                                return new JoinVisitorImpl<REALSXP, REALSXP>( left, right) ;    
                            }
                            
                            break ;    
                        }  
                    case INTSXP:
                        {
                            if( is_bare_vector(right) ){
                                return new JoinVisitorImpl<REALSXP, INTSXP>( left, right) ;    
                            }
                        }
                    default: break ;
                    }
                    
                }
            case LGLSXP:  
                {
                    switch( TYPEOF(right) ){
                    case LGLSXP:
                        {
                            return new JoinVisitorImpl<LGLSXP,LGLSXP> ( left, right ) ;       
                        }
                    case INTSXP:
                        {
                            if( is_bare_vector(right) ){
                                return new JoinVisitorImpl<LGLSXP, INTSXP>( left, right ) ;    
                            }
                            break ;
                        }
                    case REALSXP: 
                        {
                            if( is_bare_vector(right) ){
                                return new JoinVisitorImpl<LGLSXP, REALSXP>( left, right ) ;    
                            }
                        }
                    default: break ;
                    }
                    break ;
                }
            case STRSXP:  
                {
                    switch( TYPEOF(right) ){
                    case INTSXP:
                        {
                            if( Rf_inherits(right, "factor" ) ){
                                return new JoinStringFactorVisitor( left, right ) ;    
                            }
                            break ;
                        }
                    case STRSXP:
                        {
                            return new JoinVisitorImpl<STRSXP,STRSXP> ( left, right ) ;
                        }
                    default: break ;
                    }
                    break ;
                }
            default: break ;
        }
        
        incompatible_join_visitor(left, right, name) ;
        return 0 ;
    }

}
