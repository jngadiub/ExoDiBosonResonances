void makeNoBrXsect()
{
	ifstream inFile ("xsect_BulkG_WW_lvjj_c0p5_xsect_in_pb.txt") ;
	ofstream outFile("xsect_BulkG_WW_c0p5_xsect_in_pb.txt");
	string buffer;
	while (!inFile.eof ())
	{
		getline (inFile, buffer) ;
		//cout<<buffer.c_str()<<endl;
		if (buffer != "")
		{
			stringstream line ( buffer );
			string mass;
			string xsec;
			line>>mass;
			line>>xsec;
			outFile<<mass.c_str()<<"	"<<atof(xsec.c_str())/0.292032<<endl;
		}
	}
}
