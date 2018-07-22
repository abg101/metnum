/*
Matriz<double> generarRuido(Matriz<double> limpio)
{
	Matriz<double> conRuido(limpio);
	for(int i = 0; i < conRuido.filas(); i++)
	{
		int ruido = conRuido[i][0] + (std::rand() % 256);

		if(ruido > 255)
		{
			ruido = 255;
		}

		conRuido[0][i] = ruido;

	}
	return conRuido;
}
*/
double gaussianNumber()
{
	double v1,v2,s;

	do {
		v1 = 2.0 * ((double) rand()/RAND_MAX) - 1;
		v2 = 2.0 * ((double) rand()/RAND_MAX) - 1;

		s = v1*v1 + v2*v2;
	} while ( s >= 1.0 );

	if (s == 0.0)
		return 0.0;
	else
		return (v1*sqrt(-2.0 * log(s) / s));
}

Matriz<double> generarRuido(Matriz<double> signal)
{
	Matriz<double> conRuido(signal);
	for(int i = 0; i < conRuido.filas(); i++)
	{
		conRuido[i][0] += gaussianNumber();
	}
	return conRuido;
}
