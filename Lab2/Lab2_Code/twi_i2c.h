#ifndef TWI_I2C_H
#define TWI_I2C_H


#define TWBR_400kHz   12
#define TWPS_400kHz   0

#define I2C_WRITE   0x00
#define I2C_READ    0x01

#define STATUS_MASK     0xF8

#define MT_STATUS_START       0x08
#define MT_STATUS_RSTART      0x10
#define MT_STATUS_SLA_WR_ACK  0x18
#define MT_STATUS_SLA_WR_NACK 0x20
#define MT_STATUS_DATA_ACK    0x28
#define MT_STATUS_DATA_NACK   0x30
#define MT_STATUS_ARB_LOST    0x38

#define MT_SLA_WR_ACK_SUCCESS   0
#define MT_START_FAIL           1
#define MT_SLA_WR_NACK          2
#define MT_SLA_WR_FAIL          3

#define MR_STATUS_START       0x08
#define MR_STATUS_RSTART      0x10
#define MR_STATUS_ARB_LOST    0x38  
#define MR_STATUS_SLA_RD_ACK  0x40
#define MR_STATUS_SLA_RD_ACK  0x48
#define MR_STATUS_DATA_ACK    0x50
#define MR_STATUS_DATA_NACK   0x58

#define MR_SLA_RD_ACK_SUCCESS   0
#define MR_RSTART_FAIL          1
#define MR_SLA_RD_NACK          2
#define MR_SLA_RD_FAIL          3

#define GET_BIT_RATE_DIVIDER(r)   (uint8_t)(((F_CPU)/(r*2)) - (8))

class I2C {
  
  private:
    uint8_t _bit_rate;
    uint8_t _bit_rate_div;
    void _internal_start (void);

  public:
    I2C (uint8_t rate);
    ~I2C (void);
    
    uint8_t get_bit_rate(void);
    uint8_t get_bit_rate_divider(void);
    void set_bit_rate (uint8_t rate);

    uint8_t start (uint8_t addr);
    uint8_t reapeat_start (uint8_t addr);
    uint8_t write (uint8_t data);
    uint8_t read (bool send_ack = false);
    void stop (void);
    
};

#endif
