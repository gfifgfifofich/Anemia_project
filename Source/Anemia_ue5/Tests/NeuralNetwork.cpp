// Fill out your copyright notice in the Description page of Project Settings.


#include "NeuralNetwork.h"
#include <thread>
#include <algorithm>
#include <execution>
#include <iostream>
#include <fstream>
#include <strstream>


NeuralNetwork::NeuralNetwork()
{
}

NeuralNetwork::~NeuralNetwork()
{
    Delete();
}

float sigmoid(float x)
{
	return 1 / (1 + pow(2.71828, -x));
}

float sigmoidApprox(float x)
{
	if (x > -2.05f && x < 2.05f)
		return x * 0.2f + 0.5f;

	if (x < -5)
		return 0.0f;

	if (x > 5)
		return 1.0f;

	if (x < -2.05f)
		return x * 0.03f + 0.15f;

	if (x > 2.05f)
		return x * 0.03f + 0.85f;

	return 0.0f;
}
float ReLu(float x)
{
	return x >= 0.0f ? x : 0.0f;
}

void NeuralNetwork::Create(int* Architecture, int archsize)
{
	//layers = new Layer[archsize];
	LayersAmount = archsize;
	sizein = Architecture[0];
	sizeout = Architecture[archsize - 1];


	int NN_Size = 0;
	for (int i = 0; i < archsize; i++)
		NN_Size += Architecture[i];

	if (NN_Size == 0)
		std::cerr << "NN err1: Invalid size";

	int Weights_Size = 0;

	for (int i = 1; i < archsize; i++)
		Weights_Size += Architecture[i - 1] * Architecture[i];

	if (Weights_Size == 0)
		std::cerr << "NN err2: Invalid weights size";

	outputs = new float[sizeout];
	Nodes = new float[NN_Size];

	biases = new float[NN_Size];
	biasGradients = new float[NN_Size];


	weights = new float[Weights_Size];
	weightGradients = new float[Weights_Size];
	weightOutputs = new float[Weights_Size];


	NodesAmount = NN_Size;
	WeightsAmount = Weights_Size;

	Arch = new int[archsize];
	NodesStep = new int[archsize];
	WeightsStep = new int[archsize];
	//memcpy(Arch, Architecture, sizeof(int) * archsize);

	int sum = 0;
	for (int i = 0; i < archsize; i++)
	{
		Arch[i] = Architecture[i];
		NodesStep[i] = sum;
		sum += Architecture[i];
	}
	sum = 0;
	WeightsStep[0] = 0;
	for (int i = 1; i < archsize; i++)
	{
		WeightsStep[i] = sum;
		sum += Arch[i - 1] * Arch[i];
	}

	Created = true;


}

void NeuralNetwork::Delete()
{
	delete[] outputs;
	delete[] Nodes;
	delete[] biases;
	delete[] biasGradients;


	delete[] weights;
	delete[] weightGradients;
	delete[] weightOutputs;


	delete[] Arch;
	delete[] NodesStep;
	delete[] WeightsStep;
	Created = false;
}

void NeuralNetwork::Randomize()
{
	for (int i = 0; i < WeightsAmount; i++)
		weights[i] = ((rand() % 1000) / 1000.0f - 0.5f) * 2.0f;

	for (int i = 0; i < NodesAmount; i++)
		biases[i] = ((rand() % 1000) / 1000.0f - 0.5f) * 2.0f;
	lastCost = -1;

}
void NeuralNetwork::Run(float* inputData)
{
	inputs = inputData;

	for (int i = 0; i < sizein; i++)
		Nodes[i] = inputData[i];

	int PrevNodeStart = 0;
	for (int i = 1; i < LayersAmount; i++)
	{
		int size = Arch[i];
		int weightsPerNode = Arch[i - 1];
		int node = 0;
		for (int n = NodesStep[i]; n < NodesStep[i] + size; n++)
		{//each node of layer

			//WeightsStep[i-1]

			float sum = 0.0f;
			int start = (WeightsStep[i - 1] + node * weightsPerNode);
			int PrevLayerNode = 0;
			for (int w = start; w < start + weightsPerNode; w++)
			{
				weightOutputs[i] = weights[w] * Nodes[PrevNodeStart + PrevLayerNode];
				sum += weightOutputs[i];

				PrevLayerNode++;
			}

			Nodes[n] = sigmoidApprox(sum + biases[n]);
			node++;
		}
		PrevNodeStart = NodesStep[i];
	}
	for (int i = 0; i < sizeout; i++)
		outputs[i] = Nodes[PrevNodeStart + i];
}
void NeuralNetwork::Run(float ActFunc(float), float* inputData)
{
	inputs = inputData;

	for (int i = 0; i < sizein; i++)
		Nodes[i] = inputData[i];

	int PrevNodeStart = 0;
	for (int i = 1; i < LayersAmount; i++)
	{
		int size = Arch[i];
		int weightsPerNode = Arch[i - 1];
		int node = 0;
		for (int n = NodesStep[i]; n < NodesStep[i] + size; n++)
		{//each node of layer

			//WeightsStep[i-1]

			float sum = 0.0f;
			int start = (WeightsStep[i - 1] + node * weightsPerNode);
			int PrevLayerNode = 0;
			for (int w = start; w < start + weightsPerNode; w++)
			{
				weightOutputs[i] = weights[w] * Nodes[PrevNodeStart + PrevLayerNode];
				sum += weightOutputs[i];

				PrevLayerNode++;
			}

			Nodes[n] = ActFunc(sum + biases[n]);
			node++;
		}
		PrevNodeStart = NodesStep[i];
	}
	for (int i = 0; i < sizeout; i++)
		outputs[i] = Nodes[PrevNodeStart + i];
}

// inputs an array through a NN and calculates diviation from output 
float NeuralNetwork::Cost(float* input, float* output, int amount)
{
	float sum = 0.0f;


	for (int i = 0; i < amount; i++)
	{
		float* arr = new float[sizein];

		for (int iter = 0; iter < sizein; iter++)
			arr[iter] = input[i * sizein + iter];


		Run(arr);

		delete[] arr;

		float tmp = 0.0f;
		for (int a = 0; a < sizeout; a++)
		{
			tmp = outputs[a] - output[i * sizeout + a];

			tmp = tmp * tmp;
			sum += tmp;
		}
	}

	sum /= amount;
	return sum;
}
void NeuralNetwork::ApplyGrad()
{
	for (int i = 0; i < WeightsAmount; i++)
		weights[i] -= weightGradients[i];
	for (int i = 0; i < NodesAmount; i++)
		biases[i] -= biasGradients[i];


}
void NeuralNetwork::DeApplyGrad()
{
	for (int i = 0; i < WeightsAmount; i++)
		weights[i] += weightGradients[i];
	for (int i = 0; i < NodesAmount; i++)
		biases[i] += biasGradients[i];
}

void NeuralNetwork::SetupLearing()
{

	for (int i = 0; i < WeightsAmount; i++)
		weightGradients[i] = weights[i] * h * ((rand() % 100000) / 100000.0f - 0.5f) * 2.0f;
	for (int i = 0; i < NodesAmount; i++)
		biasGradients[i] = biases[i] * h * ((rand() % 100000) / 100000.0f - 0.5f) * 2.0f;
	ApplyGrad();
}
void NeuralNetwork::ApplyLearing(float cost)
{
	if(lastCost < cost)
		DeApplyGrad();
    else
        lastCost = cost;
}

void NeuralNetwork::CopyInto(NeuralNetwork* target)
{
	if(target->Created)
		target->Delete();
	target->Create(Arch,LayersAmount);

	for (int i = 0; i < WeightsAmount; i++)
		target->weights[i] = weights[i];

	for (int i = 0; i < NodesAmount; i++)
		target->biases[i] = biases[i];
		
}
void NeuralNetwork::CopyIntoWithGradient(NeuralNetwork* target)
{
	if(target->Created)
		target->Delete();
	target->Create(Arch,LayersAmount);

	for (int i = 0; i < WeightsAmount; i++)
		{
			target->weights[i] = weights[i] + weightGradients[i] * h * (((rand() % 100000) / 100000.0f) * 2.0f - 1.0f);
			target->weightGradients[i] = weightGradients[i];
		}

	for (int i = 0; i < NodesAmount; i++)
		{
			target->biases[i] = biases[i] + biasGradients[i] * h * (((rand() % 100000) / 100000.0f) * 2.0f - 1.0f);
			target->biasGradients[i] = biasGradients[i];
		}
}
// Uses Cost() to minimize the diviation. Method - finite difirences
void NeuralNetwork::learn(float Rate, float* input, float* output, int amount)
{
	rate = Rate;
	h = 0.0001f;
	c1 = Cost(input, output, amount);


	for (int i = 0; i < WeightsAmount; i++)
	{
		weights[i] += h;

		float c2 = Cost(input, output, amount);
		weights[i] -= h;
		float slope = (c2 - c1) / h;

		weightGradients[i] = slope * rate;
	}
	for (int i = 0; i < NodesAmount; i++)
	{
		biases[i] += h;

		float c2 = Cost(input, output, amount);
		biases[i] -= h;
		float slope = (c2 - c1) / h;

		biasGradients[i] = slope * rate;
	}


	ApplyGrad();
	float cst = Cost(input, output, amount);

	float cst2 = cst;
	bool run = true;
	while (run)
	{
		run = false;
		ApplyGrad();
		cst2 = Cost(input, output, amount);
		if (cst2 < cst)
		{
			run = true;
			cst = cst2;
		}
		else
			DeApplyGrad();
	}
}
//std::string throutputs[12];


void NeuralNetwork::Customlearn(float Costfunc(NeuralNetwork*), float Learnrate, bool finitediff)
{
	rate = Learnrate;
	c1 = Costfunc(this);
	std::vector<int> iter;

	iter.resize(threadcount);
	for (int i = 0; i < threadcount; i++)
		iter[i] = i;

	for (int i = 0; i < DataStorage.size(); i++)
		DataStorage[i]->Delete();

	DataStorage.clear();


		DataStorage.resize(gyms);
		for (int i = 0; i < DataStorage.size(); i++)
		{

			DataStorage[i] = new NeuralNetwork;
			DataStorage[i]->Create(Arch, LayersAmount);
		}
		for(auto thr : iter)
			{
				int amount = gyms / threadcount;
				int start = (thr)*amount;
				int end = (thr + 1) * amount;
				if (thr == threadcount - 1)
					end = gyms;
				for (int s = start; s < end; s++)
				{/*
					DataStorage[s] = new NeuralNetwork;
					DataStorage[s]->Create(Arch, LayersAmount);*/
					for (int i = 0; i < WeightsAmount; i++)
						DataStorage[s]->weights[i] = weights[i] + h * ((rand() % 1000) / 1000.0f - 0.5f) * 2.0f;

					for (int i = 0; i < NodesAmount; i++)
						DataStorage[s]->biases[i] = biases[i] + h * ((rand() % 1000) / 1000.0f - 0.5f) * 2.0f;

					DataStorage[s]->lastCost = Costfunc(DataStorage[s]);
				}



			}
		float mincost = lastCost;
		NeuralNetwork* bestNN = NULL;
		for (int i = 0; i < DataStorage.size(); i++)
			if (DataStorage[i]->lastCost < mincost || (mincost < 0.0f && DataStorage[i]->lastCost >= 0.0f))
			{
				mincost = DataStorage[i]->lastCost;
				bestNN = DataStorage[i];
			}
		if (mincost == lastCost)
			bestNN = this;
		if ((mincost < lastCost && bestNN != NULL) || lastCost < 0.0f) {
			lastCost = mincost;

			for (int i = 0; i < WeightsAmount; i++)
			{
				weightGradients[i] = (weights[i] - bestNN->weights[i]) * Learnrate;
				//weights[i] = DataStorage[minIndex]->weights[i];
			}
			for (int i = 0; i < NodesAmount; i++)
			{
				biasGradients[i] = (biases[i] - bestNN->biases[i]) * Learnrate;
				//biases[i] = DataStorage[minIndex]->biases[i];
			}
			
		}
		else
		{
			for (int i = 0; i < WeightsAmount; i++)
			{
				weightGradients[i] = 0.0f;
				//weights[i] = DataStorage[minIndex]->weights[i];
			}
			for (int i = 0; i < NodesAmount; i++)
			{
				biasGradients[i] = 0.0f;
				//biases[i] = DataStorage[minIndex]->biases[i];
			}
		}

		/*	for (int i = 0; i < DataStorage.size(); i++)
				DataStorage[i]->~NeuralNetwork();

			DataStorage.clear();*/
	

	


	ApplyGrad();
	float cst = Costfunc(this);

	float cst2 = cst;
	bool run = true;
	while (run)
	{
		run = false;
		ApplyGrad();
		cst2 = Costfunc(this);
		if (cst2 < cst)
		{
			run = true;
			cst = cst2;
		}
		else
			DeApplyGrad();
	}
	lastCost = cst;

	for (int i = 0; i < DataStorage.size(); i++)
		DataStorage[i]->Delete();

	DataStorage.clear();
}

void NeuralNetwork::SaveTo(std::string filename)
{
	std::ofstream SaveFile(filename);
	SaveFile << LayersAmount;
	SaveFile << " " << WeightsAmount;
	SaveFile << " " << NodesAmount;
	SaveFile << "\n";

	for (int i = 0; i < LayersAmount; i++)
		SaveFile << Arch[i] << "\n";
	for (int i = 0; i < WeightsAmount; i++)
		SaveFile << weights[i] << "\n";
	for (int i = 0; i < NodesAmount; i++)
		SaveFile << biases[i] << "\n";
	SaveFile.close();

}
void NeuralNetwork::LoadFrom(std::string filename)
{
	Delete();

	std::ifstream f(filename);
	if (!f.is_open())
	{
		std::cerr << "ERROR LOADING NN: Unable to open " << filename;
		return;
	}
	int i = 0;
	int state = 0;
	while (!f.eof())
	{

		char junk;
		char line[32];
		f.getline(line, 32);
		std::strstream s;
		s << line;
		if (state == 0)
		{
			s >> LayersAmount >> WeightsAmount >> NodesAmount;
			Arch = new int[LayersAmount];
			state = 1;
		}
		else if (state == 1)
		{
			s >> Arch[i];

			i++;
			if (i >= LayersAmount)
			{
				i = 0;
				state = 2;

				int* Archbuff = Arch;

				Create(Arch, LayersAmount);
				delete[] Archbuff;
			}
		}
		else if (state == 2)
		{
			s >> weights[i];
			i++;
			if (i >= WeightsAmount)
			{
				i = 0;
				state = 3;
			}
		}
		else if (state == 3)
		{
			s >> biases[i];

			i++;
			if (i >= NodesAmount)
			{
				i = 0;
				state = 4;
			}
		}
	}
	f.close();
}
