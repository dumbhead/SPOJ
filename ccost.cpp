#include <iostream>
#include <algorithm>
#include <string.h>
#include <stdio.h>
#include <map>
#include <limits.h>
using namespace std;

#define MAXX 200000
#define MAXQ 50000
#define LL long long int
#define BUFSIZE (10000)
#define putchar(c) (*outptr++=(c))
#define getchar() ({if(ioptr>=ioend)init_input();*ioptr++;})
#define eof() (ioptr>=ioend&&input_eof)
#define eoln() ({if(ioptr>=ioend)init_input();*ioptr=='\n';})

int N, Q;

struct event {
       int x, y, val, d, rank;
} e [MAXX];

int arrx [MAXX];
map <int, int> px;
pair <int, int> q [MAXQ];
LL tree [4 * MAXX], result[MAXQ];

 
char outputbuffer [BUFSIZE<<1], inputbuffer [BUFSIZE];
char *outptr = outputbuffer, *ioptr = inputbuffer + BUFSIZE, *ioend = inputbuffer + BUFSIZE;
int input_eof = 0;
 
 
void init_input(){
  if (input_eof)
		return;
	int existing = BUFSIZE - (ioend - inputbuffer);
	memcpy (inputbuffer, ioend, existing);
	int wanted = ioend - inputbuffer;
	int count = fread (inputbuffer + existing, 1, wanted, stdin);
	if (count < wanted)
		input_eof = 1;
	ioend = inputbuffer + BUFSIZE - (wanted - count);
	while(*--ioend > ' ');
	ioend++;
	ioptr = inputbuffer;
}

inline void non_whitespace (){
	for(;;){
		if(ioptr >= ioend)
			init_input ();
		if(*ioptr > ' ')
			return;
		ioptr++;
	}
}
 
void flush_output (){
	fwrite (outputbuffer, 1, outptr - outputbuffer, stdout);
	outptr = outputbuffer;
}
 
inline void check_output(){
	if(outptr>=outputbuffer+BUFSIZE)
		flush_output();
}

inline int getint(){
	non_whitespace();
	int neg = 0;
	if(*ioptr == '-'){
		ioptr++;
		neg = 1;
	}
	
	int n = 0;
	while(*ioptr>' ')
		n = (n<<3) + (n<<1) + *ioptr++ - '0';
	ioptr++;
	if(neg)
		n = -n;
	return n;
}
 
inline void putlong(LL n){
	if(n < 0){
		*outptr++='-';
		n = -n;
	}
	char buffer [30];
	int i = 0;
    LL q;
	do{
		q = n/10;
		buffer [i++] = n - (q<<3) - (q<<1) + '0';
	}while(n = q);
	while(i)
		putchar (buffer [--i]);
}

LL query(int indx, int begin, int end, int start, int finish){
     if(begin > finish || end < start)
              return 0;
              
     else if(begin >= start && end <= finish)
          return tree[indx];
          
     else
          return query(2 * indx, begin, (begin + end)/2, start, finish) + query(2 * indx + 1, (begin + end)/2 + 1, end, start, finish);
}

void update(int indx, int begin, int end, int pos, int val){
     if(begin == pos && end == pos)
              tree[indx] += val;

     else{
          if(pos > (begin + end)/2)
                 update(2 * indx + 1, (begin + end)/2 + 1, end, pos, val);
          else
                 update(2 * indx, begin, (begin + end)/2, pos, val);
          
          tree[indx] = tree[2 * indx] + tree[2 * indx + 1];
     }
}

bool compa (struct event a, struct event b) {
     if (a.y < b.y)
        return true;
     
     else if (a.y > b.y)
        return false;
        
     else {
          if (a.d < b.d)
             return true;
          
          else if (a.d > b.d)
              return false;
              
          else {
               if (a.x < b.x)
                  return true;
                  
               else
                   return false;
          }
     }
}

int main () {
    int T;
    int indx;
    
    T = getint ();
    while (T--) {
        indx = 0;  
        
        N = getint ();
        for (int i = 0; i < N; i++) {
                 
                 e [i].x = getint ();
                 e [i].y = getint ();
                 e [i].val = getint ();
                 
                 e[i].d = 0;
                 e[i].rank = i + 1;
                 
                 arrx [indx++] = e [i].x;
        }
        
        Q = getint ();
        
        for (int i = 0; i < Q; i++) {
                 
                 e [N + 2 * i].x = getint ();
                 e [N + 2 * i].y = getint () - 1;
                 e [N + 2 * i].d = 1;
                 e[N + 2 * i].rank = N + i + 1;
                 
                 arrx [indx++] = e [N + 2 * i].x;
                 
                 e [N + 2 * i + 1].x = getint ();
                 e [N + 2 * i + 1].y = getint ();
                 e [N + 2 * i + 1].d = 2;
                 e[N + 2 * i + 1].rank = N + i + 1;
                 
                 arrx [indx++] = e [N + 2 * i + 1].x;
                 
                 q [i].first = e [N + 2 * i].x;
                 q [i].second = e [N + 2 * i + 1].x;
        }
        
        sort (arrx, arrx + indx);
        
        int id = 1;
        for(int i = 0; i < indx; i++){
                if(!px [arrx [i]]){
                       px [arrx [i]] = id;
                       id++;
                }
        }
        
        sort (e, e + N + 2 * Q, compa);
        
        for (int i = 0; i < N + 2 * Q; i++) {
             if (e[i].d == 0)
                update (1, 1, px [arrx [indx - 1]], px [e [i].x], e [i].val);
                
             else if (e[i].d == 1)
                  result [e[i].rank - N - 1] = -query (1, 1, px [arrx [indx - 1]], px [q [e[i].rank - N - 1].first], px [q [e[i].rank - N - 1].second]);
                  
             else
                  result [e[i].rank - N - 1] += query (1, 1, px [arrx [indx - 1]], px [q [e[i].rank - N - 1].first], px [q [e[i].rank - N - 1].second]);
        }
        
        for (int i = 0; i < Q; i++) {
            putlong (result[i]);
            putchar ('\n');
            check_output();
        }
        
        flush_output();   
        
        px.clear ();
        memset (tree, 0, sizeof (tree));
    }
        
    return 0;
}
