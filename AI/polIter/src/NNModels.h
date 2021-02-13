//So this only works when libtorch is available, which in my case is only on the linux machines on which I 
//run this. 
#pragma once
#ifdef __linux__
#define TorchAvailable true
#endif
#ifdef TorchAvailable
#include <torch/torch.h>
#include <fstream>
#include <string>

struct LogitModelImpl : torch::nn::Module {
	LogitModelImpl(int64_t N, int64_t M)
	{
		W = register_parameter("W", torch::zeros({ N,M }));
		b = register_parameter("b", torch::zeros(M));
	}
	torch::Tensor forward(torch::Tensor input) {
		return torch::addmm(b, input, W);
	}
	torch::Tensor W, b;

};
TORCH_MODULE(LogitModel);

struct LayerModelImpl : torch::nn::Module {

	LayerModelImpl()
		:linear1 (1,1), linear2(1,1),linear3(1,1), linear4(1,1), linear5(1,1), linear6(1,1)
	{
	}
	LayerModelImpl(int64_t N, int64_t M, std::vector<int64_t> hidden)
		:linear1(N, (hidden.size()>0 ? hidden[0]: M )), 
		linear2((hidden.size() > 0 ? hidden[0] : M), (hidden.size() > 1 ? hidden[1] : M)), 
		linear3((hidden.size() > 1 ? hidden[1] : M), (hidden.size() > 2 ? hidden[2] : M)),
		linear4((hidden.size() > 2 ? hidden[2] : M), (hidden.size() > 3 ? hidden[3] : M)),
		linear5((hidden.size() > 3 ? hidden[3] : M), (hidden.size() > 4 ? hidden[4] : M)),
		linear6((hidden.size() > 4 ? hidden[4] : M), M),
		numHiddenLayer{ hidden.size() }
	{
		Register();
	}
	void Register()
	{		

		register_module("linear1", linear1);
		if (numHiddenLayer > 0)
		{
			register_module("linear2", linear2);
		}
		if (numHiddenLayer > 1)
		{
			register_module("linear3", linear3);
		}
		if (numHiddenLayer > 2)
		{
			register_module("linear4", linear4);
		}
		if (numHiddenLayer > 3)
		{
			register_module("linear5", linear5);
		}
		if (numHiddenLayer > 4)
		{
			register_module("linear6", linear6);
		}
	}
	//There must be a better way for this, but I can't find it. 
	torch::Tensor forward(torch::Tensor input) {
		switch (numHiddenLayer){
		case 0:
			return linear1(input);
		case 1:
			return linear2(torch::relu(linear1(input)));
		case 2:
			return linear3(torch::relu(linear2(torch::relu(linear1(input)))));
		case 3:
			return linear4(torch::relu(linear3(torch::relu(linear2(torch::relu(linear1(input)))))));
		case 4:
			return linear5(torch::relu( linear4(torch::relu(linear3(torch::relu(linear2(torch::relu(linear1(input)))))))));
		case 5:
			return linear6(torch::relu( linear5(torch::relu(linear4(torch::relu(linear3(torch::relu(linear2(torch::relu(linear1(input)))))))))));
		default:
			std::cout << "incorrect number of hidden layers" << std::endl;
			throw;
		}		
	}
	void SaveMetaInfo(std::string loc)
	{
		std::ofstream of;
		std::string outputfile{ loc };
		of.open(outputfile);
		of << numHiddenLayer << "\t";
	}
	void LoadMetaInfo(std::string loc)
	{
		std::ifstream ifs;
		std::string inputfile{ loc };
		ifs.open(inputfile);
		ifs >> numHiddenLayer;
		Register();
	}
	torch::nn::Linear linear1, linear2, linear3, linear4, linear5, linear6;

	size_t numHiddenLayer;
};
TORCH_MODULE(LayerModel);

#endif