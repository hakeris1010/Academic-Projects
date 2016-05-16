#ifndef F_H_INCLUDED
#define F_H_INCLUDED

typedef int TYPE;

TYPE add(TYPE n1, TYPE n2);
TYPE sub(TYPE n1, TYPE n2);

TYPE exec(TYPE (*fun)(TYPE, TYPE), TYPE a1, TYPE a2);

int getAddCount();
int getSubCount();
int totalCount();

#endif // F_H_INCLUDED
