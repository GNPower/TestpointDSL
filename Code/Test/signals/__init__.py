from abc import ABC, abstractmethod
import numpy as np

class Signal(ABC):
    def __init__(self, length: int = 100, stride: float = 0.1) -> None:
        self.length = length
        self.stride = stride
        self.x = np.arange(0, length+stride, stride)
    
    def get_x(self):
        return self.x
    
    def get_x_len(self):
        return len(self.x)

    @abstractmethod
    def evaluate(self, x: float):
        pass
    
    def noise(self, std: float = 1):
        return np.random.normal(0, std, len(self.x))

    def getf_Exact(self):
        return [self.evaluate(i) for i in self.x]
    
    def getf_GaussianNoise_y(self, std: float = 1):
        noise = self.noise(std)
        return [self.evaluate(self.x[i])+noise[i] for i in range(len(self.x))]
    
    def getf_GaussianNoise_x(self, std: float = 1):
        noise = self.noise(std)
        return [self.evaluate(self.x[i]+noise[i]) for i in range(len(self.x))]
    
    def getf_GaussianNoise_xy(self, std: float = 1):
        noisex = self.noise(std)
        noisey = self.noise(std)
        return [self.evaluate(self.x[i]+noisex[i])+noisey[i] for i in range(len(self.x))]
    
    def getf_Delayed(self, delay: int = 1):
        return [ 
            self.evaluate(self.x[i-delay]) 
            if i > delay 
            else self.evaluate(self.x[-delay+i])
            for i in range(len(self.x)) 
        ]
    
    def getf_Early(self, early: int = 1):
        return [
            self.evaluate(self.x[(i+early)%len(self.x)])
            if i >= len(self.x) - early
            else self.evaluate(self.x[i+early])
            for i in range(len(self.x))
        ]
    
    def getf_DelayedNoise(self, delay: int = 1, std: float = 1):
        noisex = self.noise(std)
        noisey = self.noise(std)
        return [ 
            self.evaluate(self.x[i-delay]+noisex[i])+noisey[i] 
            if i > delay 
            else self.evaluate(self.x[-delay+i]+noisex[i])+noisey[i] 
            for i in range(len(self.x)) 
        ]
    
    def getf_EarlyNoise(self, early: int = 1, std: float = 1):
        noisex = self.noise(std)
        noisey = self.noise(std)
        return [
            self.evaluate(self.x[(i+early)%len(self.x)]+noisex[i])+noisey[i]
            if i >= len(self.x) - early
            else self.evaluate(self.x[i+early]+noisex[i])+noisey[i]
            for i in range(len(self.x))
        ]