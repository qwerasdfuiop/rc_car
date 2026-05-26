/*
 * MPU6050.c
 *
 *  Created on: Oct 6, 2022
 *      Author: mac
 */
#include "MPU6050.h"
#include "main.h"

#define MPU6050_ADDR 0xD0

#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75

int16_t Accel_X_RAW, Accel_Y_RAW, Accel_Z_RAW = 0;
int16_t Gyro_X_RAW, Gyro_Y_RAW, Gyro_Z_RAW = 0;
float Ax, Ay, Az = 0;
float Gx, Gy, Gz = 0;

uint8_t MPU_Rx_Buffer[14]; // 가속도~자이로까지 통째로 담을 버퍼
volatile uint8_t mpu_data_ready = 0; // 데이터 수신 완료 플래그


void MPU6050_Init (void)
{
  uint8_t check, Data;

  //check device ID WHO_AM_I
  HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR, WHO_AM_I_REG, 1, &check, 1, 1000);
  LL_USART_TransmitData8(USART2, check);

  //wake sensor up
  if (check == 104) //if the device is present
  {
    // power management register (0x6B) we should write all 0's to wake the sensor up
    Data = 0;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, PWR_MGMT_1_REG, 1, &Data, 1, 1000);

    // Set the DATA RATE of 1KHz by writing SMPLRT_DIV register
    Data = 0x07;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, SMPLRT_DIV_REG, 1, &Data, 1, 1000);

    // Set accelerometer configuration in ACCEL_CONFIG Register
    // XA_ST=0, YG_ST=0, ZG_ST=0, FS_SEL=0 -> +- 250 deg/s
    Data = 0x00;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, 1000);

    // Set Gyroscope configuration in GYRO_CONFIG Register
    // XG_ST=0, YG_ST=0, ZG_ST=0, FS_SEL=0 -> +- 250deg/s
    Data = 0x00;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1, 1000);

  }
}

void MPU6050_Read_All_IT(void)
{
  // ACCEL_XOUT_H_REG(0x3B)부터 자이로 끝까지 14바이트를 비동기로 읽어옵니다.
  HAL_I2C_Mem_Read_IT(&hi2c1, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, MPU_Rx_Buffer, 14);
}

void MPU6050_Parse_Data(void)
{
  // 1. 가속도 RAW 데이터 복원 및 변환
  Accel_X_RAW = (int16_t)(MPU_Rx_Buffer[0] << 8 | MPU_Rx_Buffer[1]);
  Accel_Y_RAW = (int16_t)(MPU_Rx_Buffer[2] << 8 | MPU_Rx_Buffer[3]);
  Accel_Z_RAW = (int16_t)(MPU_Rx_Buffer[4] << 8 | MPU_Rx_Buffer[5]);
  Ax = Accel_X_RAW / 16384.0;
  Ay = Accel_Y_RAW / 16384.0;
  Az = Accel_Z_RAW / 16384.0;

  // [참고] MPU_Rx_Buffer[6], [7]은 온도 데이터(TEMP_OUT)입니다. 필요 없으면 건너뜁니다.

  // 2. 자이로 RAW 데이터 복원 및 변환
  Gyro_X_RAW = (int16_t)(MPU_Rx_Buffer[8] << 8 | MPU_Rx_Buffer[9]);
  Gyro_Y_RAW = (int16_t)(MPU_Rx_Buffer[10] << 8 | MPU_Rx_Buffer[11]);
  Gyro_Z_RAW = (int16_t)(MPU_Rx_Buffer[12] << 8 | MPU_Rx_Buffer[13]);
  Gx = Gyro_X_RAW / 131.0;
  Gy = Gyro_Y_RAW / 131.0;
  Gz = Gyro_Z_RAW / 131.0;
}
