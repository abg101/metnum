#include "Matriz.h"


Matriz::Matriz()
{
    filas = 0;
    columnas = 0;
}

Matriz::Matriz(long filas, long columnas)
{
    this->construct(filas, columnas);
}

Matriz::~Matriz()
{
   
}

long Matriz::Filas()
{
    return filas;
}

long Matriz::Columnas()
{
    return columnas;
}

void Matriz::construct(long filas, long columnas)
{
    if (filas < 1 || columnas < 1) {
        throw "La dimension de la matriz no puede ser cero o negativa";
    }

    this->filas = filas;
    this->columnas = columnas;

    vals = vector<double> (); 
    cols = vector<long> ();
    filas_ptr =  vector<long>(filas + 1, 1);
}


Matriz & Matriz::Set(double val, long fila, long col)
{
    this->validarCoordenadas(fila, col);

    long pos = filas_ptr.at(fila - 1) - 1;
    long actual = -1;
    //recorro la fila en el vector de cols entre los indices apuntados por filas_ptr y 
    // el de la fila siguiente.
    for (; pos < filas_ptr.at(fila) - 1; pos++) {
        actual = cols.at(pos);
//en la posicion pos de cols me indica que columna esta almacenada.
        if (actual == col) {
            break; //la quiero ya estaba.

        } else if (actual > col) {
            break; //la que quiero no estaba.
        }
    }

    if (actual != col) {
        if (val != 0) {
            this->insert(pos, fila, col, val); //mete el valor.
        } //si no estaba y quiero meter un cero no hago nada.

    } else if (abs(val) < 0.00000001) {
        this->remove(pos, fila); //si quiero ehcufar un cero saco esa posicion de los vectores val y cols.

    } else {
        vals.at(pos) = val; //si no y si ya estaba meto el valor.
    }

    return *this;
}



double Matriz::Get(long fila, long col) const
{
    this->validarCoordenadas(fila, col);

    long actual;
    //recorro la fila en el vector de cols entre los indices apuntados por filas_ptr y el de la fila siguiente.
    for (long i = filas_ptr.at(fila - 1) - 1; i < filas_ptr.at(fila) - 1; i++) {
        actual = cols.at(i); //en la pos i de cols me indica que columna esta almacenada.

        if (actual == col) { //si es igual a la buscada retorno el elem de vals en esa pos.
            return vals.at(i);

        } else if (actual > col) { //si es mayor es porque en la posicion buscada hay cero.
            break;
        }
    }

    return 0;
}

vector<double> Matriz::multiply(const vector<double> & x) const
{
    if (this->columnas != (long) x.size()) {
        throw "No matchea la dimension";
    }

    vector<double> result(this->filas, 0);
    long actual = 1;

   //recorro el vector de no ceros.
   for (long i = 0; i < vals.size(); ++i)
   {      
        if (i == (filas_ptr.at(actual) -1))
        {
            actual ++;
            long j = filas_ptr.at(actual-1);
            while (j == filas_ptr.at(actual))
            {
                actual ++;
            }
        }

       long columna = cols.at(i);
       result[actual-1] += (vals.at(i))*x[columna-1];

      
   }    //

    return result;
}


Matriz Matriz::multiply(const Matriz & b) const
{
    if (this->columnas != b.filas) {
        throw "No matchea la dimension";
    }

    Matriz result(this->filas, b.columnas);

    double a;

    for (long i = 1; i <= this->filas; i++) {
        for (long j = 1; j <= b.columnas; j++) {
            a = 0;

            for (long k = 1; k <= this->columnas; k++) {
                a += this->Get(i, k) * b.Get(k, j);
            }

            result.Set(a, i, j);
        }
    }

    return result;
}

void imprimirvector80(std::vector<double> v) {
    for (int i = 0; i < v.size(); ++i)
    {
        cout << v[i] << "," << endl;
    }
}

vector<double> Matriz::valores(){
   // vector<double> res = vals
    return this->vals;
}

void Matriz::escalar(double k)
{
    //Matriz result(this->filas, this->columnas)   

    for (int i = 0; i < vals.size(); i++)
    {
        (this->vals)[i]= (double)(k*(this->vals)[i]);
     
    }

}


Matriz Matriz::add(const Matriz & mat) const
{
    if (this->filas != mat.filas|| this->columnas != mat.columnas) {
        throw "No matchea dimension";
    }

    Matriz result(this->filas, this->columnas);

    for (int i = 1; i <= this->filas; i++) {
        for (int j = 1; j <= this->columnas; j++) {
            result.Set(this->Get(i, j) + mat.Get(i, j), i, j);
        }
    }

    return result;
}

Matriz Matriz::sub(const Matriz & mat) const
{
    if (this->filas != mat.filas || this->columnas != mat.columnas) {
        throw "No matchea dimension";
    }

    Matriz result(this->filas, this->columnas);

    for (int i = 1; i <= this->filas; i++) {
        for (int j = 1; j <= this->columnas; j++) {
            result.Set(this->Get(i, j) - mat.Get(i, j), i, j);
        }
    }

    return result;
}


void Matriz::validarCoordenadas(long fila, long col) const
{
    if (fila < 1 || col < 1 || fila > this->filas || col > this->columnas) {
        throw "Coordenadas fuera del rango";
    }
}


void Matriz::insert(long index, long fila, long col, double val)
{
    if (vals.empty()) {
        vals =  vector<double>(1, val);
        cols =  vector<long>(1, col);
     //si no habia nada creo los vectores vals y columnas con sus respectivos elems
    } else {
        vals.insert(vals.begin() + index, val);
        cols.insert(cols.begin() + index, col);
    } //si no, ingreso en la posicion deseada .

    for (long i = fila; i <= this->filas; i++) {
        filas_ptr.at(i) = filas_ptr.at(i) + 1;
    } //todos los indices de las filas que le siguen se corren . inc a cant de no ceros.
}


void Matriz::remove(long index, long fila)
{
    vals.erase(vals.begin() + index); //borro esa posicion en vals
    cols.erase(cols.begin() + index); //borro esa posicion en cols

    for (long i = fila; i <= this->filas; i++) {
        filas_ptr.at(i) = filas_ptr.at(i) - 1;
    } //los indices de las filas que le siguen ahora son uno menos al igual que los no ceros.
}

bool operator == (const Matriz & a, const Matriz & b)
{
    return ((a.vals).empty() && b.vals.empty())
                || (!(a.vals).empty() && !(b.vals).empty() && (a.vals) == (b.vals))
            && (((a.cols).empty() && (b.cols).empty())
                || (!(a.cols).empty() && !(b.cols).empty() && (a.cols) == (b.cols)))
            && (a.filas_ptr) == (b.filas_ptr);
}


bool operator != (const Matriz & a, const Matriz & b)
{
    return !(a == b);
}


ostream & operator << (ostream & os, const Matriz & matrix)
{
    for (int i = 1; i <= matrix.filas; i++) {
        for (int j = 1; j <= matrix.columnas; j++) {
            if (j != 1) {
                os << " ";
            }

            os << matrix.Get(i, j);
        }

        if (i < matrix.filas) {
            os << endl;
        }
    }

    return os;
}

Matriz operator * (const Matriz & a, const Matriz & b) {
    
    return a.multiply(b);
}

vector<double> operator * (const Matriz & a, const vector<double> & b) {

    return a.multiply(b);
}

Matriz operator + (const Matriz & a, const Matriz & b){
  
  return a.add(b);

}

Matriz operator - (const Matriz & a, const Matriz & b){

    return a.sub(b);
}


Matriz Matriz::transpose()
{
	long m = this->filas;
	long n = this->columnas;

	Matriz res(n,m);
	for(long i = 1; i <= m; i++)
		for(long j = 1; j <= n; j++)
			res.Set(this->Get(i,j),j,i);
	return res;
}

vector<double> Matriz::transmultiply(const vector<double> & x) const
{
    if (this->filas != (int) x.size()) {
        throw "No matchea la dimension";
    }

    vector<double> result(this->columnas, 0);
    long actual = 1;

   //recorro el vector de no ceros.
   for (long i = 0; i < vals.size(); ++i)
   {      
        if (i == (filas_ptr.at(actual) -1))
        {
            actual ++;
            long j = filas_ptr.at(actual-1);
            while (j == filas_ptr.at(actual))
            {
                actual ++;
            }
        }

       long columna = cols.at(i);
       result[columna-1] += (vals.at(i))*x[actual-1];

      
   }    //

    return result;

}
