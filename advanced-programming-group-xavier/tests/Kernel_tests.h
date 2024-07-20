#ifndef ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_KERNEL_TESTS_H
#define ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_KERNEL_TESTS_H

void testKernelDefaultInstantiation();
void testKernelInstantiationNoSigma();
void testKernelInstantiationWithSigma();
void testKernelCalculatePaddingOddKernelSize();
void testKernelCalculatePaddingEvenKernelSize();
void testKernelGenerateMedianKernelOdd();
void testKernelGenerateMedianKernelEven();
void testKernelGenerateBoxKernelOdd();
void testKernelGenerateBoxKernelEven();
void testKernelGenerateGaussianKernelOdd();
void testKernelGenerateGaussianKernelEven();
void testKernelSettersAndGetters();
void testKernelApplyKernelMedian3by3();
void testKernelApplyKernelBox2by2();
void testKernelApplyKernelGaussian3by3();

#endif //ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_KERNEL_TESTS_H
