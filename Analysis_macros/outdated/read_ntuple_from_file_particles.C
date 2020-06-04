// Read the previously produced N-Tuple and print on screen
// its contentvoid 
#include"TCanvas.h"
#include"TROOT.h"
#include"TGraphErrors.h"
#include"TLegend.h"
#include <vector>
using namespace std;

void read_ntuple_from_file_particles(){
	// to make the graph
	vector<float> nv;
	vector<float> ev; 
	TCanvas canvas("canvas","part_spec",700,500);
	canvas.SetGrid();

	// Open a file, save the ntuple and close the file
	TFile in_file("part_spec_run2.root");
	TNtuple* my_tuple;
	in_file.GetObject("part_spec_data",my_tuple);
	float n,e;
	float* row_content;
	cout << "E\tN\n";

	for(int irow=0;irow<my_tuple->GetEntries();++irow){
	my_tuple->GetEntry(irow);
	row_content = my_tuple->GetArgs();
	e = row_content[0];
	n = row_content[1];

	nv.push_back(n);
	ev.push_back(e);

	cout << e << "\t" << n << endl;
 	}

	int size = ev.size();
	TGraphErrors error_graph(size,ev.data(),nv.data(),nullptr,nullptr);
	error_graph.SetMarkerColor(kRed);
	error_graph.SetMarkerStyle(20);
	error_graph.SetTitle("Electrons production spectrum;Energy MeV; Proportional of electrons produced");
	error_graph.Draw("APL");
	canvas.Draw();
}
