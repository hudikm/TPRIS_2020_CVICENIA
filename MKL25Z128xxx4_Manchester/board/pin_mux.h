/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/*! @name PORTA1 (number 27), RX_PIN
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_RX_PIN_FGPIO FGPIOA             /*!<@brief FGPIO peripheral base pointer */
#define BOARD_INITPINS_RX_PIN_GPIO GPIOA               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_RX_PIN_GPIO_PIN_MASK (1U << 1U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_RX_PIN_PORT PORTA               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_RX_PIN_PIN 1U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_RX_PIN_PIN_MASK (1U << 1U)      /*!<@brief PORT pin mask */
                                                       /* @} */

/*! @name PORTA2 (number 28), OUT_PIN
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_OUT_PIN_FGPIO FGPIOA             /*!<@brief FGPIO peripheral base pointer */
#define BOARD_INITPINS_OUT_PIN_GPIO GPIOA               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_OUT_PIN_GPIO_PIN_MASK (1U << 2U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_OUT_PIN_PORT PORTA               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_OUT_PIN_PIN 2U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_OUT_PIN_PIN_MASK (1U << 2U)      /*!<@brief PORT pin mask */
                                                        /* @} */

/*! @name PORTC7 (number 64), OUT2_PIN
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_OUT2_PIN_FGPIO FGPIOC             /*!<@brief FGPIO peripheral base pointer */
#define BOARD_INITPINS_OUT2_PIN_GPIO GPIOC               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_OUT2_PIN_GPIO_PIN_MASK (1U << 7U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_OUT2_PIN_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_OUT2_PIN_PIN 7U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_OUT2_PIN_PIN_MASK (1U << 7U)      /*!<@brief PORT pin mask */
                                                         /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
