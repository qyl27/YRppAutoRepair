#include <BuildingClass.h>
#include <HouseClass.h>
#include <EventClass.h>
#include <TargetClass.h>
#include <Fundamentals.h>

#include <Syringe.h>

static constexpr int scan_frame_interval = 20;

static int last_scan_frame = 0;

DEFINE_HOOK(0x55B4E1, LogicClass_Update_AutoRepair, 0x5)
{
	const int current_frame = Unsorted::CurrentFrame;

	if (abs(current_frame - last_scan_frame) < scan_frame_interval)
	{
		return 0;
	}

	last_scan_frame = current_frame;

	if (HouseClass* p_player = HouseClass::CurrentPlayer)
	{
		for (BuildingClass* p_building : p_player->Buildings)
		{
			if (!p_building || !p_building->IsAlive)
			{
				continue;
			}
			if (p_building->IsBeingRepaired)
			{
				continue;
			}
			if (p_building->Health >= p_building->GetType()->Strength)
			{
				continue;
			}

			EventClass ev(p_player->ArrayIndex, EventType::Repair);
			ev.Repair.Whom = TargetClass(p_building);
			EventClass::OutList.Add(ev);
		}
	}

	return 0;
}
