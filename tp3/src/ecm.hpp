double ecm(Matriz<double> orig, Matriz<double> reconstruido)
{
	int n = orig.filas();
	double acum = 0;
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			double diferencia = orig[i][j] - reconstruido[i][j];
			acum += diferencia*diferencia;
		}
	}
	return acum/(n*n);
}

