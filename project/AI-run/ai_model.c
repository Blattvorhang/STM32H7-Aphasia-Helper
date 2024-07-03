#include <stdio.h>

#include "network.h"
#include "network_data.h"
#include "data_processing.h"
#include "main.h"

/* Global handle to reference the instantiated C-model *//*����ʵ������C-model��ȫ�־��*/
static ai_handle network = AI_HANDLE_NULL;

/* Global c-array to handle the activations buffer *//*���ڴ����������ȫ��c����*/
AI_ALIGNED(32)
static ai_u8 activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];

/* Array to store the data of the input tensor *//*���ڴ洢�����������ݵ�����*/
AI_ALIGNED(32)
// static ai_float in_data[AI_NETWORK_IN_1_SIZE];
static ai_i8 in_data[AI_NETWORK_IN_1_SIZE_BYTES]; 

/* c-array to store the data of the output tensor *//*���ڴ洢����������ݵ�c����*/
AI_ALIGNED(32)
// static ai_float out_data[AI_NETWORK_OUT_1_SIZE];
static ai_i8 out_data[AI_NETWORK_OUT_1_SIZE_BYTES];

/* Array of pointer to manage the model's input/output tensors */
/*���ڹ���ģ������/���������ָ������*/
static ai_buffer *ai_input;
static ai_buffer *ai_output;

/* 
 * Bootstrap
 */
int aiInit(void) {
  ai_error err;
  
  /* Create and initialize the c-model *//*��������ʼ��c-model */
  const ai_handle acts[] = { activations };
  err = ai_network_create_and_init(&network, acts, NULL);
  if (err.type != AI_ERROR_NONE) {  };

  /* Reteive pointers to the model's input/output tensors *//*��ȡָ��ģ������/���������ָ��*/
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
  
  /* 1 - Update IO handlers with the data payload *//* 1 -��������Ч�غɸ���IO�������*/
  ai_input[0].data = AI_HANDLE_PTR(in_data);
  ai_output[0].data = AI_HANDLE_PTR(out_data);

  /* 2 - Perform the inference *//* 2 -ִ������*/
  n_batch = ai_network_run(network, &ai_input[0], &ai_output[0]);
  if (n_batch != 1) {
      err = ai_network_get_error(network);
  };
  
  return 0;
}

void aiInference() {
  /* The STM32 CRC IP clock should be enabled to use the network runtime library */
  /* Ӧ����STM32 CRC IPʱ�ӣ���ʹ����������ʱ�� */
  __HAL_RCC_CRC_CLK_ENABLE();

  aiInit();

	/* 1 - Acquire, pre-process and fill the input buffers */
	/* 1 -��ȡ��Ԥ�����������뻺����*/
	acquire_and_process_data(in_data);

	/* 2 - Call inference engine */
	/* 2 -������������*/
	aiRun(in_data, out_data);

	/* 3 - Post-process the predictions */
	/* 3������Ԥ����к���*/
	post_process(out_data);
}
