#include <stdio.h>

#include "network.h"
#include "network_data.h"
#include "data_processing.h"
#include "main.h"

/* Global handle to reference the instantiated C-model *//*引用实例化的C-model的全局句柄*/
static ai_handle network = AI_HANDLE_NULL;

/* Global c-array to handle the activations buffer *//*用于处理激活缓冲区的全局c数组*/
AI_ALIGNED(32)
static ai_u8 activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];

/* Array to store the data of the input tensor *//*用于存储输入张量数据的数组*/
AI_ALIGNED(32)
// static ai_float in_data[AI_NETWORK_IN_1_SIZE];
static ai_i8 in_data[AI_NETWORK_IN_1_SIZE_BYTES]; 

/* c-array to store the data of the output tensor *//*用于存储输出张量数据的c数组*/
AI_ALIGNED(32)
// static ai_float out_data[AI_NETWORK_OUT_1_SIZE];
static ai_i8 out_data[AI_NETWORK_OUT_1_SIZE_BYTES];

/* Array of pointer to manage the model's input/output tensors */
/*用于管理模型输入/输出张量的指针数组*/
static ai_buffer *ai_input;
static ai_buffer *ai_output;

/* 
 * Bootstrap
 */
int aiInit(void) {
  ai_error err;
  
  /* Create and initialize the c-model *//*创建并初始化c-model */
  const ai_handle acts[] = { activations };
  err = ai_network_create_and_init(&network, acts, NULL);
  if (err.type != AI_ERROR_NONE) {  };

  /* Reteive pointers to the model's input/output tensors *//*获取指向模型输入/输出张量的指针*/
  ai_input = ai_network_inputs_get(network, NULL);
  ai_output = ai_network_outputs_get(network, NULL);

  return 0;
}

/* 
 * Run inference
 */
int aiRun(const void *in_data, void *out_data) {
  ai_i32 n_batch;
  ai_error err;
  
  /* 1 - Update IO handlers with the data payload *//* 1 -用数据有效载荷更新IO处理程序*/
  ai_input[0].data = AI_HANDLE_PTR(in_data);
  ai_output[0].data = AI_HANDLE_PTR(out_data);

  /* 2 - Perform the inference *//* 2 -执行推理*/
  n_batch = ai_network_run(network, &ai_input[0], &ai_output[0]);
  if (n_batch != 1) {
      err = ai_network_get_error(network);
  };
  
  return 0;
}

void aiInference() {
  /* The STM32 CRC IP clock should be enabled to use the network runtime library */
  /* 应启用STM32 CRC IP时钟，以使用网络运行时库 */
  __HAL_RCC_CRC_CLK_ENABLE();

  aiInit();

	/* 1 - Acquire, pre-process and fill the input buffers */
	/* 1 -获取、预处理和填充输入缓冲区*/
	acquire_and_process_data(in_data);

	/* 2 - Call inference engine */
	/* 2 -调用推理引擎*/
	aiRun(in_data, out_data);

	/* 3 - Post-process the predictions */
	/* 3——对预测进行后处理*/
	post_process(out_data);
}
