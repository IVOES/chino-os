using System;
using System.Collections.Generic;
using System.Text;

namespace Chino.Driver
{
    public class Stm32F10xUsartDriver : IDriverDefinitionProvider
    {
        public static readonly Guid Usart = new Guid("D108B4B6-CBE3-4497-8E45-E826C8D06D58");

        private readonly List<DriverDefinition> _drivers = new List<DriverDefinition>();

        public IReadOnlyCollection<DriverDefinition> Drivers => _drivers;

        public Stm32F10xUsartDriver()
        {
            _drivers.Add(new DriverDefinition
            {
                Id = Usart,
                Name = "stm32f10x-usart",
                Compatible = new[] { Chip.STM32F1XX_HD.DeviceCompatibles.Usart },
                Path = "serial/st/stm32f10x-usart"
            });
        }
    }
}
