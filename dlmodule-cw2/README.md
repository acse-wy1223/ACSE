## DL module coursework 2

<img src="https://drive.google.com/uc?id=1DG7dis2Daa9MObZP7pK5HBMBQiHMED-D" width="700"/>

Relase date: 14:00h Thursday 14th December 2023

Submission deadline: 18:00h Friday 15th December 2023

**At 18:00h on Friday I will clone the repositories, so make sure to have everything commited by then.**

<br>



<span style="color: red;">Please download the best model https://drive.google.com/file/d/1fbg0LMa9t1bwD5Zna0dwvIZnZMv4zDMR/view?usp=drive_link to dir `./model_best` before running the notebook</span>.

## **Tasks**

### Task 1
Classify the hand images in the `test_hands` folder using any method you have learned during the module. The classifier should classify test hands using the following labels:

- `0`: real hand
- `1`: VAE hand
- `2`: GAN hand

<br>

### Task2
Answer the two questions at the end of this README file. Answer them in the README itself and commit and push your modified README file to your repository.

<br>



## Deliverables

Your final repository should contain the following:

1. `yourusername_classified_hands.csv`: a comma-separated value file with the name of the hand in the test set, then a comma, and then the prediction for this hand image. It should look like:

	```
	test_hand_0001.jpeg, 2
	test_hand_0002.jpeg, 0
	...
	```
with the appropriate lable values that your classifier generates. Add this new file to the repo and do not put it inside any subfolder please.

2. `yourusername_DLcw2_clean.ipynb`: a clean notebook with your classifier workflow (similar to the one you created in coursework 1), should include data preparation, network implementation, final training, final evaluation of the test set, writing of the csv file, and any other step you have done that is not related to hyperparameter tunning or network design tests (only steps using your best and final classifier model). 

3. `yourusername_DLcw2_hypertune.ipynb`: as in coursework 1, a notebook where you explore your best hyperparameter values for your final network design and training that you will include in `yourusername_DLcw2_clean.ipynb`.

**[IMPORTANT]: I do not provide templates this time, you can design it as you wish, but you will be assessed on the clarity and tidiness of your submitted notebooks. And remember that  all code cells should be executed before you commit and push the notebooks to the repo.**

4. `README.md` — Modified version of this README file that includes your answers to the questions you will find below. Please stick to the word limit, I will not assess anything written beyond the given word limit.

5. `references.md` — List of your references you have used in similar to the one you provided in coursework 1. You have to create the markdown file yourself this time.

<br>

## **Task 1 - Classifier**

### Datasets
You will find the following folders in this repository:
- `real_hands`: contains real hands (but feel free to use more from the first coursework repo if you think it is a good idea.
- `VAE_hands`: contains hands generated using VAEs.
- `GAN_hands`: contains hands generared using GANs.
- `test_hands`: contains a mix of real hands, VAE-generated hands, and GAN-generated hands.

Use the four datasets as you see fit to design and implement a classifier to label the images in the `test_hands` folder as:

- real hands (label: `0`)
- VAE-generated hands (label: `1`)
- GAN-generated hands (label: `2`)

You have to implement your network explicitly and train it from randomly initialised weights (no transfer learning), but you can use any architecture you think is useful.

<br>

## **Task 2 - Questions**
Modify this README file to include the answers to the questions below.

### Question 1 [150 words maximum]
If I asked you to do the first coursework again, but now I gave you 5 days and 500 compute units (instead of 100 compute units), what would you do differently to improve your results. In this case, you would be able to use any architecture we have seen in class. Justify your answers well, and do not go over the word limit. You can use bullet points and a maximum of one diagram/figure/table to support your answer.

### ANSWER to question 1:

If I have enough time and computational units, I would build a GAN model based on the diffusion model (Diffusion-GAN) <a href="#[1]">[1]</a>, which can reduce the artifacts and refine the details while considering the diversity of images, which would be a better model for the medical image generation with strict requirements on texture details like the X-ray hands.

M. Ozbey et al proposed a new method, an adversarial diffusion modeling (SynDiff) for Medical Image Translation in 2022 and significantly improved the fidelity of the generated images <a href="#[2]">[2]</a>; similarly, in 2023, C. Tiago et al. proposed combining DDM with GAN and generated high quality image samples with faster sampling time <a href="#[3]">[3]</a>.

Therefore I would choose Diffusion-GAN to complete coursework1. I will also try various Data-Augmentation methods to preprocess images to achieve better results. 

<center><img src=".\assets\image-20231215142049767.png" alt="image-20231215133031028" style="zoom:50%;" /></center>

<center><div style="color:orange; border-bottom: 1px solid #d9d9d9;     display: inline-block;     color: #999;     padding: 2px;">FIG1: SynDiff model on BRATS <a href="#[2]">[2]</a> and DDM-GAN model results <a href="#[3]">[3]</a></div></center>



<br>

### Question 2 [100 words maximum]
Additionally to the newly defined hypotethical assessment in **Question 1**, if i told you that now the images were 512x512 pixels instead of 32x32, and I gave you a choice between these two extra resources:

**a.** as much compute power as you want, but only have the 10000 images provided. <br>
**b.** as many more images in your dataset as you want, but only have the 500 compute units availabe.

Which one would you choose and why? Again justify your answers and use bullet points or tables as you see fit.

### ANSWER to question 2:
I will choose **a**:
1. High-resolution images require more computational resources to process due to the higher number of pixels and increased model complexity.

2. The quality of the dataset is usually more important than the quantity. If the 10,000 images provided are diverse and representative of the data distribution, then they are sufficient and the greater computational power allows advanced data augmentations to artificially increase the diversity of the dataset without requiring more images <a href="#[4]">[4]</a>. For example, the [StyleGAN2-ADA](https://github.com/NVlabs/stylegan2-ada-pytorch?tab=readme-ov-file) model of Y. Choi et al. trains excellent images on [AFHQ](https://github.com/clovaai/stargan-v2/blob/master/README.md#animal-faces-hq-dataset-afhq) (15,000 high-quality images 512 × 512 resolution <a href="#[5]">[4, 5]</a>.

   

<center><img src="https://github.com/NVlabs/stylegan2-ada-pytorch/blob/main/docs/stylegan2-ada-teaser-1024x252.png?raw=true" style="zoom:67%;" /></center>



<center><div style="color:orange; border-bottom: 1px solid #d9d9d9;     display: inline-block;     color: #999;     padding: 2px;">FIG2: StyleGAN2-ADA generated images <a href="#[4]">[4]</a></div></center>

## Reference

<a id="[1]">[1]</a> Z. Wang, H. Zheng, P. He, W. Chen, and M. Zhou, "Diffusion-GAN: Training GANs with Diffusion," 2022. [Online]. Available: [arXiv:2206.02262](https://arxiv.org/abs/2206.02262) [cs.LG].

<a id="[2]">[2]</a> Özbey, M., Dalmaz, O., Dar, S. U. H., Bedel, H. A., Özturk, Ş., Güngör, A., & Çukur, T. (2023). Unsupervised Medical Image Translation With Adversarial Diffusion Models. IEEE Transactions on Medical Imaging, 42(12), 3524-3539. doi: [10.1109/tmi.2023.3290149.](https://arxiv.org/abs/2207.08208)

<a id="[3]">[3]</a> C. Tiago, S. R. Snare, J. Šprem and K. McLeod, "A Domain Translation Framework With an Adversarial Denoising Diffusion Model to Generate Synthetic Datasets of Echocardiography Images," in *IEEE Access*, vol. 11, pp. 17594-17602, 2023, doi: [10.1109/ACCESS.2023.3246762.](https://ieeexplore.ieee.org/abstract/document/10049068)

<a id="[4]">[4]</a> T. Karras, M. Aittala, J. Hellsten, S. Laine, J. Lehtinen, and T. Aila, "Training Generative Adversarial Networks with Limited Data," *arXiv preprint arXiv:2006.06676*, 2020.

<a id="[5]">[5]</a> Y. Choi, Y. Uh, J. Yoo, and J.-W. Ha, "StarGAN v2: Diverse Image Synthesis for Multiple Domains," in *Proceedings of the IEEE/CVF Conference on Computer Vision and Pattern Recognition (CVPR)*, 2020.

