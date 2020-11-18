# Sunto

Si parte da qua [Accelerating Python on GPUs with nvc++ and Cython](https://developer.nvidia.com/blog/accelerating-python-on-gpus-with-nvc-and-cython/)

### Scaricare HPC-SDK
Fare riferimento al sito [Nvidia](https://developer.nvidia.com/nvidia-hpc-sdk-downloads).

`nvidia-smi` produce questo

```
+-----------------------------------------------------------------------------+
| NVIDIA-SMI 450.80.02    Driver Version: 450.80.02    CUDA Version: 11.0     |
|-------------------------------+----------------------+----------------------+
```

Dunque andiamo con l'ultima versione. Entrambi i files sono salvati dentro `/media/xnext/DATAEXT/nvidia/hpc-sdk/`

```
wget \
  https://developer.download.nvidia.com/hpc-sdk/20.9/nvhpc-20-9_20.9_amd64.deb \
  https://developer.download.nvidia.com/hpc-sdk/20.9/nvhpc-2020_20.9_amd64.deb
sudo apt-get install ./nvhpc-20-9_20.9_amd64.deb ./nvhpc-2020_20.9_amd64.deb
```
