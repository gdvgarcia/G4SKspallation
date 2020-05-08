// here we want to simulate the energy spectrum of muons in the SK   
#include"TCanvas.h"
#include"TROOT.h"
#include"TRandom3.h"
#include"TH1F.h"
#include"TGraphErrors.h"
#include"TMultiGraph.h"

void randon_generator() {
// Create a histogram, fill it with random spectrum energies

//for drawing both figures in the same canvas
auto canvas =new TCanvas();


//creating the spectrum 
const int n_points=10;
double x_vals[n_points]={0.1,1,2,3,10,100,200,1000,2000,4000};
double y_vals[n_points]={2e-09,1e-09,1e-09,1e-09,1e-09,4e-10,2.25e-10,1e-11,1.5e-12,1.75e-13};

//for the plot
double y_plot[n_points]={2e-09,1e-09,1e-09,1e-09,1e-09,4e-10,2.25e-10,1e-11,1.5e-12,1.75e-13};

for(int i = 0; i < 10; i++){
	y_plot[i]=1450000*0.1e+09*y_vals[i];
}

// Instance the graph to see the spectrum
TGraphErrors graph(n_points,x_vals,y_plot,nullptr,nullptr);
graph.SetTitle("Muon spectrum;Energy [MeV]; flux of muons [cm^-2 s^-1 MeV^-1]");

// Make the plot estetically better//
graph.SetMarkerStyle(20);;
graph.SetMarkerColor(kRed);
graph.SetLineColor(kRed);

//draw it
graph.DrawClone("APL");
gPad->SetLogx(1);
gPad->SetLogy(1);


//create the histogram
TH1F *histogram = new TH1F ("histogram", "Spectrum", 5000, 0,5000);
histogram->SetTitle("Muon spectrum;Energy [GeV]; # of muons");


//to count the data
int n_data = 0;

//for random data
TRandom3 my_random_generator;
double rnd = 1.;
double y_r, x_r;
int pos;

//generating data
while (n_data < 1000000){
	// A random number in [0,1]
	rnd = my_random_generator.Uniform(1.);

	//generate a random pair
	double y_r = (1e-08 - 1e-14)*rnd+1e-14;
	rnd = my_random_generator.Uniform(0,1);
	int pos = int(10*rnd);
	
	if(y_r < y_vals[pos]){

		//different smearing for different poitns
		//rnd = my_random_generator.Exp(500*x_vals[pos]);


		//if(pos > 4){
		//	rnd = my_random_generator.Exp(0.1*x_vals[pos]);
		//}
		//if(pos == 6){
		//	rnd = my_random_generator.Exp(0.01*x_vals[pos]);
		//}
		//if(pos == 7){
		//	rnd = my_random_generator.Exp(0.00001*x_vals[pos]);
		//}
		//if(pos == 8){
		//	rnd = my_random_generator.Exp(0.0000001*x_vals[pos]);
		//}
		//if(pos == 9){
		//	rnd = my_random_generator.Exp(0.00000001*x_vals[pos]);
		//}
		//if(pos = 0){
		//	rnd = my_random_generator.Exp(0.0000001*x_vals[pos]);
		//}
		// create a random smearing
		rnd = my_random_generator.Uniform(0,1);
		if(pos == 0){
			rnd = 0.1*my_random_generator.Uniform(0,1);
		}
		x_r = rnd + x_vals[pos];
		histogram->Fill(x_r);
		n_data ++;
	}
}


//draw it
histogram->SetMarkerColor(kRed);
histogram->SetLineColor(kBlue);
histogram->DrawClone("Same");
gPad->SetLogx(1);
gPad->SetLogy(1);

canvas->Draw();
}
