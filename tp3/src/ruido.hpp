
Matriz<double> generarRuido(Matriz<double> limpio)
{
	std::srand(time(NULL));
	Matriz<double> conRuido(limpio);
	for(int i = 0; i < conRuido.columnas(); i++)
	{
		int ruido = conRuido[0][i] + std::rand() % 256;

		if(ruido > 255)
		{
			ruido = 255;
		}
		conRuido[0][i] = ruido;

	}
	return conRuido;
}
