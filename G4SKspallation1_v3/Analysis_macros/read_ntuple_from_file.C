// Read the previously produced N-Tuple and print on screen
// its contentvoid 

void read_ntuple_from_file(){
	// Open a file, save the ntuple and close the file
	TFile in_file("B4.root");
	TNtuple* my_tuple;
	in_file.GetObject("B4",my_tuple);
	float Eabs,Egap,Labs,Lgap;
	float* row_content;
	cout << "Eabs\tEgap\tLabs\tLgap\n";

	for(int irow=0;irow<my_tuple->GetEntries();++irow){
	my_tuple->GetEntry(irow);
	row_content = my_tuple->GetArgs();
	Eabs = row_content[0];
	Egap = row_content[1];
	Labs = row_content[2];
	Lgap = row_content[3];

	cout << Eabs << "\t" << Egap << "\t" << Labs << "\t" << Lgap << endl;
 	}
}
