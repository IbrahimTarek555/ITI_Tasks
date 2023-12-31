/*********************************************************************************/
/*********************************************************************************/
/**********************			Author: Ibrahim Tarek		**********************/
/**********************			File: TWI_program.c			**********************/
/**********************			Version: 1.00				**********************/
/*********************************************************************************/
/*********************************************************************************/

/**
 * @file TWI_program.c
 * @author Ibrahim Tarek
 * @brief This file implements the TWI module functions.
 * @version 1.00
 */

#include "../../../Common/STD_TYPES.h"
#include "../../../Common/BIT_MATH.h"
#include "../../../Common/DEFINES.h"

#include "../Header/TWI_interface.h"
#include "../Header/TWI_config.h"
#include "../Header/TWI_private.h"
#include "../Header/TWI_register.h"

/**
 * @brief 					Function to initialize the TWI peripheral as master.
 * @param Copy_u8Address	Address which represent the peripheral.
 * @return					Error State.
 */
TWI_ERROR_STATUS_t TWI_enMasterInit(uint8 Copy_u8Address)
{
	/*Define Error State variable*/
	TWI_ERROR_STATUS_t Local_enErrorState = NO_ERROR;

	/*Validate the Address with the standards of TWI protocol*/
	if(Copy_u8Address <= 127u)
	{
		if(((Copy_u8Address >> 3) != 15u) || (Copy_u8Address != 0u))
		{
			/*Set the Address into its register*/
			TWAR = Copy_u8Address << 1u;

			/*Enable Acknowledge*/
			SET_BIT(TWCR,TWCR_TWEA);

			/*Set the Communication Frequency to 400 KHz*/
			CLR_BIT(TWSR,TWSR_TWPS0);
			CLR_BIT(TWSR,TWSR_TWPS1);
			TWBR = 2u;

			/*Enable TWI*/
			SET_BIT(TWCR,TWCR_TWEN);
		}
		else
		{
			Local_enErrorState = RESERVED_ADDRESS_ERROR;
		}
	}
	else
	{
		Local_enErrorState = FETAL_ERROR;
	}

	/*Return Error State*/
	return Local_enErrorState;
}

/**
 * @brief					Function to initialize the TWI peripheral as slave.
 * @param Copy_u8Address	Address which represent the peripheral.
 * @return					Error State.
 */
TWI_ERROR_STATUS_t TWI_enSlaveInit(uint8 Copy_u8Address)
{
	/*Define Error State variable*/
	TWI_ERROR_STATUS_t Local_enErrorState = NO_ERROR;

	/*Validate the Address with the standards of TWI protocol*/
	if(Copy_u8Address > 127u)
	{
		if(((Copy_u8Address >> 3) != 15u) || (Copy_u8Address != 0u))
		{
			/*Set the Address into its register*/
			TWAR = Copy_u8Address << 1u;

			/*Enable Acknowledge*/
			SET_BIT(TWCR,TWCR_TWEA);

			/*Enable TWI*/
			SET_BIT(TWCR,TWCR_TWEN);
		}
		else
		{
			Local_enErrorState = RESERVED_ADDRESS_ERROR;
		}
	}
	else
	{
		Local_enErrorState = FETAL_ERROR;
	}

	/*Return Error State*/
	return Local_enErrorState;
}

/**
 * @brief 	Function to send start condition.
 * @return	Error State.
 */
TWI_ERROR_STATUS_t TWI_enSendStartCondition(void)
{
	/*Define Error State variable*/
	TWI_ERROR_STATUS_t Local_enErrorState = NO_ERROR;

	/*Set Start Condition*/
	SET_BIT(TWCR,TWCR_TWSTA);

	/*Clear the interrupt flag to enable the previous action to happen*/
	SET_BIT(TWCR,TWCR_TWINT);

	/*Wait until the previous action finished and the interrupt flag is raised again*/
	while((GET_BIT(TWCR,TWCR_TWINT)) == 0u);

	/*Check the status register*/
	if((TWSR & STATUS_REGISTER_MASK) != START_ACK)
	{
		Local_enErrorState = START_CONDITION_ERROR;
	}
	else
	{

		/*Do nothing*/
	}

	/*Return Error State*/
	return Local_enErrorState;
}

/**
 * @brief	Function to send a repeated start.
 * @return	Error State.
 */
TWI_ERROR_STATUS_t TWI_enSendRepeatedStart(void)
{
	/*Define Error State variable*/
	TWI_ERROR_STATUS_t Local_enErrorState = NO_ERROR;

	/*Set Start Condition*/
	SET_BIT(TWCR,TWCR_TWSTA);

	/*Clear the interrupt flag to enable the previous action to happen*/
	SET_BIT(TWCR,TWCR_TWINT);

	/*Wait until the previous action finished and the interrupt flag is raised again*/
	while((GET_BIT(TWCR,TWCR_TWINT)) == 0u);

	/*Check the status register*/
	if((TWSR & STATUS_REGISTER_MASK) != REP_START_ACK)
	{
		Local_enErrorState = REP_START_CONDITION_ERROR;
	}
	else
	{
		/*Do nothing*/
	}

	/*Return Error State*/
	return Local_enErrorState;
}

/**
 * @brief						Function to send slave address with write.
 * @param Copy_u8SlaveAddress	Slave Address.
 * @return						Error state.
 */
TWI_ERROR_STATUS_t TWI_enSendSlaveAddressWithWrite(uint8 Copy_u8SlaveAddress)
{
	/*Define Error State variable*/
	TWI_ERROR_STATUS_t Local_enErrorState = NO_ERROR;

	/*Set the slave address into the data register*/
	TWDR = Copy_u8SlaveAddress << 1u;

	/*Clear bit 0 for write operation*/
	CLR_BIT(TWDR,0u);

	/*Clear Start Condition*/
	CLR_BIT(TWCR,TWCR_TWSTA);

	/*Clear the interrupt flag to enable the previous action to happen*/
	SET_BIT(TWCR,TWCR_TWINT);

	/*Wait until the previous action finished and the interrupt flag is raised again*/
	while((GET_BIT(TWCR,TWCR_TWINT)) == 0u);

	/*Check the status register*/
	if((TWSR & STATUS_REGISTER_MASK) != SLAVE_ADD_AND_WR_ACK)
	{
		Local_enErrorState = SLAVE_ADD_WITH_WR_ERROR;
	}
	else
	{
		/*Do nothing*/
	}

	/*Return Error State*/
	return Local_enErrorState;
}

/**
 * @brief						Function to send slave address with read.
 * @param Copy_u8SlaveAddress	Slave Address.
 * @return						Error State.
 */
TWI_ERROR_STATUS_t TWI_enSendSlaveAddressWithRead(uint8 Copy_u8SlaveAddress)
{
	/*Define Error State variable*/
	TWI_ERROR_STATUS_t Local_enErrorState = NO_ERROR;

	/*Set the slave address into the data register*/
	TWDR = Copy_u8SlaveAddress << 1u;

	/*Set bit 0 for read operation*/
	SET_BIT(TWDR,0u);

	/*Clear Start Condition*/
	CLR_BIT(TWCR,TWCR_TWSTA);

	/*Clear the interrupt flag to enable the previous action to happen*/
	SET_BIT(TWCR,TWCR_TWINT);

	/*Wait until the previous action finished and the interrupt flag is raised again*/
	while((GET_BIT(TWCR,TWCR_TWINT)) == 0u);

	/*Check the status register*/
	if((TWSR & STATUS_REGISTER_MASK) != SLAVE_ADD_AND_RD_ACK)
	{
		Local_enErrorState = SLAVE_ADD_WITH_RD_ERROR;
	}
	else
	{
		/*Do nothing*/
	}

	/*Return Error State*/
	return Local_enErrorState;
}

/**
 * @brief				Function to send data byte.
 * @param Copy_u8Data	Data Byte.
 * @return				Error State.
 */
TWI_ERROR_STATUS_t TWI_enMasterWriteDataByte(uint8 Copy_u8Data)
{
	/*Define Error State variable*/
	TWI_ERROR_STATUS_t Local_enErrorState = NO_ERROR;

	/*Set the data byte into the data register*/
	TWDR = Copy_u8Data;

	/*Clear the interrupt flag to enable the previous action to happen*/
	SET_BIT(TWCR,TWCR_TWINT);

	/*Wait until the previous action finished and the interrupt flag is raised again*/
	while((GET_BIT(TWCR,TWCR_TWINT)) == 0u);

	/*Check the status register*/
	if((TWSR & STATUS_REGISTER_MASK) != MSTR_WR_BYTE_ACK)
	{
		Local_enErrorState = MASTER_WR_BYTE_WITH_ACK_ERROR;
	}
	else
	{
		/*Do nothing*/
	}

	/*Return Error State*/
	return Local_enErrorState;
}

/**
 * @brief 				Function to read data byte.
 * @param Copy_pu8Data	Pointer to return the data byte read.
 * @return				Error State.
 */
TWI_ERROR_STATUS_t TWI_enMasterReadDataByte(uint8 *Copy_pu8Data)
{
	/*Define Error State variable*/
	TWI_ERROR_STATUS_t Local_enErrorState = NO_ERROR;

	/*Validate the pointer to data that will be returned*/
	if(Copy_pu8Data != NULL)
	{
		/*Clear the interrupt flag to enable the slave to send data*/
		SET_BIT(TWCR,TWCR_TWINT);

		/*Wait until the slave finished writing the data and the interrupt flag is raised again*/
		while((GET_BIT(TWCR,TWCR_TWINT)) == 0u);

		/*Check the status register*/
		if((TWSR & STATUS_REGISTER_MASK) != MSTR_RD_BYTE_WITH_ACK)
		{
			Local_enErrorState = MASTER_RD_BYTE_WITH_ACK_ERROR;
		}
		else
		{
			/*Return the data received to the user*/
			*Copy_pu8Data = TWDR;
		}
	}
	else
	{
		Local_enErrorState = NULL_PTR_ERROR;
	}

	/*Return Error State*/
	return Local_enErrorState;
}

/**
 * @brief Function to send stop condition.
 */
void TWI_enSendStopCondition(void)
{
	/*Set stop Condition*/
	SET_BIT(TWCR,TWCR_TWSTO);

	/*Clear the interrupt flag to enable the previous action to happen*/
	SET_BIT(TWCR,TWCR_TWINT);
}
