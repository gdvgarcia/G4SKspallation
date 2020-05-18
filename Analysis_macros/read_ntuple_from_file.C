// Read the previously produced N-Tuple and print on screen
// its contentvoid 

void read_ntuple_from_file(){
	// Open a file, save the ntuple and close the file
	TFile in_file("eDep_run1.root");
	TNtuple* my_tuple;
	in_file.GetObject("eDep_data",my_tuple);
	float x,y,z,E, e;
	float* row_content;
	cout << "X\tY\tZ\tE\te\n";

	for(int irow=0;irow<my_tuple->GetEntries();++irow){
	my_tuple->GetEntry(irow);
	row_content = my_tuple->GetArgs();
	x = row_content[0];
	y = row_content[1];
	z = row_content[2];
	E = row_content[3];
	e = row_content[4];

	cout << x << "\t" << y << "\t" << z << "\t" << E << "\t" << e << endl;
 	}
}
