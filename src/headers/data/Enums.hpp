#ifndef __ENUMS__H__
#define __ENUMS__H__

namespace Omnia
{
	enum class eDirection {
		None = 0,
		Up,
		Left,
		Down,
		Right,
		Falling,
		Jumping
	};

	enum class eTileUnitType {
		Normal = 0,
		Animation,
		RandomAnimation
	};

	enum class eErrorLevel {
		NoError = 0,
		Info,
		Suggestion,
		Warning,
		Error,
		Critical
	};

	enum class eSolidLevel {
		FullSolid = 0,
		MediumSolid,
		NoSolid,
		FullLiquid,
		MediumLiquid
	};

	enum class eCollisionLevel {
		HardCollision = 0,
		MediumCollision,
		HardLiquidCollision,
		MediumLiquidCollision,
		NoCollision
	};

	enum class eCollisionType {
		AABB = 0
	};

	enum class eMonths {
		January = 0,
		February,
		March,
		April,
		May,
		June,
		July,
		August,
		September,
		October,
		November,
		December
	};

	enum class eDataBlockType {
		Invalid = 0,
		Int,
		Float,
		String,
		IntArray,
		StringArray,
		Literal
	};

	enum class eControlType {
		Unknown = 0,
		Keyboard,
		MouseButton,
		Joystick,
	};

    enum class eGuiComponentOrderCompare
    {
        Under = 0,
        Same,
        Over,
        Invalid
    };

	enum class eCommandResult
	{
		Ok = 0,
		Error
	};

	enum class eLightType {
		Normal = 0,
		Glowing
	};

	enum class eLightStyle {
		Normal = 0,
		Colored
	};

	enum class eLightState {
		On = 0,
		Off
	};

	enum class eNamespaceMemberType {
		Integer = 0,
		String,
		Color,
		Object,
		
		Null
	};

}
#endif
