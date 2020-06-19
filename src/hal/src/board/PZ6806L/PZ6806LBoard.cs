using System;
using System.Collections.Generic;
using System.Text;

namespace Chino.Board
{
    public class PZ6806LBoard : IBoardDefinitionProvider
    {
        public static readonly Guid Win32 = new Guid("DB4A97D2-1CEB-4C6A-AED4-6861C1F990A1");

        private readonly List<BoardDefinition> _boards = new List<BoardDefinition>();

        public IReadOnlyCollection<BoardDefinition> Boards => _boards;

        public PZ6806LBoard()
        {
            _boards.Add(new BoardDefinition
            {
                Id = Win32,
                Chip = Chip.STM32F1XX_HD.STM32F103ZET6,
                Drivers = new[]
                {
                    Driver.Stm32F10xUsartDriver.Usart
                },
                SelectedPinGroups = new Dictionary<string, string>
                {
                    { "Usart1", "Usart1-A" }
                }
            });
        }
    }
}
