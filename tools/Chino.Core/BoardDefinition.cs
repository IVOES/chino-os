using System;
using System.Collections.Generic;
using System.Text;

namespace Chino
{
    public class BoardDefinition
    {
        public Guid Id { get; set; }

        public Guid Chip { get; set; }

        public Dictionary<string, string> SelectedPinGroups { get; set; } = new Dictionary<string, string>();

        public IReadOnlyList<Guid> Drivers { get; set; } = Array.Empty<Guid>();
    }

    public interface IBoardDefinitionProvider
    {
        IReadOnlyCollection<BoardDefinition> Boards { get; }
    }
}
