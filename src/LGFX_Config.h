#pragma once

#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ILI9341 _panel;
  lgfx::Bus_SPI _bus;

public:

  LGFX(void)
  {
    {
      auto cfg = _bus.config();

      cfg.spi_host = VSPI_HOST;
      cfg.spi_mode = 0;

      cfg.freq_write = 27000000;
      cfg.freq_read = 16000000;

      cfg.spi_3wire = false;
      cfg.use_lock = true;

      cfg.dma_channel = SPI_DMA_CH_AUTO;

      cfg.pin_sclk = 18;
      cfg.pin_mosi = 23;
      cfg.pin_miso = -1;

      cfg.pin_dc = 2;

      _bus.config(cfg);
      _panel.setBus(&_bus);
    }

    {
      auto cfg = _panel.config();

      cfg.pin_cs = 15;
      cfg.pin_rst = 4;

      cfg.panel_width = 240;
      cfg.panel_height = 320;

      cfg.offset_x = 0;
      cfg.offset_y = 0;

      cfg.offset_rotation = 0;

      _panel.config(cfg);
    }

    setPanel(&_panel);
  }
};