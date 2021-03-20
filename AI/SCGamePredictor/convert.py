import numpy as np
from pytorch2keras import pytorch_to_keras
import torch
import tensorflow as tf
from tensorflow import keras
from SCGamePredictor import SCGameRecommender

input_np = np.random.uniform(0, 1, (1, 18))
input_var = torch.FloatTensor(input_np)

model = SCGameRecommender("model_SupplyChainGame_4_4.pt")

k_model = pytorch_to_keras(model, input_var, [(18, )], verbose=True)

k_model.save("model.keras")

x = np.array([model.initial_state()])
