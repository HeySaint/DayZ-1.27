class ActionAnimateSeats: ActionAnimateCarSelection
{
	void ActionAnimateSeats()
	{
		m_Text = "#move_seat";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if( !target ) return false;
		if( !IsInReach(player, target, UAMaxDistances.DEFAULT) ) return false;

		Object targetObject = target.GetObject();
		Entity targetEnt = Entity.Cast(targetObject);
		Transport transport;
		CarScript car;
		int crewIdx;

		//string selection = targetObject.GetActionComponentName(target.GetComponentIndex());
		array<string> selections = new array<string>();
		targetObject.GetActionComponentNameList(target.GetComponentIndex(), selections);

		
		if ( !Class.CastTo(transport, targetObject) )
			return false;

		if ( Class.CastTo(car, targetEnt) )
		{
			for (int i = 0; i < selections.Count(); i++)
			{
				m_AnimSource = car.GetAnimSourceFromSelection( selections[i]) ;
				if ( m_AnimSource != "" )
				{
					//return true; //TODO:: NEED A LINK BETWEEN SeatBack and seat with crew after that we can REMOVE the return
					
					HumanCommandVehicle vehCmd = player.GetCommand_Vehicle();

					if ( m_AnimSource == "SeatDriver" )
					{
						if ( transport.CrewMember( DayZPlayerConstants.VEHICLESEAT_DRIVER ) )
							return false;

						if ( !vehCmd && car.GetCarDoorsState( "NivaDriverDoors" ) == CarDoorState.DOORS_CLOSED )
							return false;
					}
					
					if ( m_AnimSource == "SeatCoDriver" )
					{
						if ( transport.CrewMember( DayZPlayerConstants.VEHICLESEAT_CODRIVER ) )
							return false;

						if ( !vehCmd && car.GetCarDoorsState( "NivaCoDriverDoors" ) == CarDoorState.DOORS_CLOSED )
							return false;
					}
					
					if ( !vehCmd && !transport.CanReachSeatFromDoors(selections[i], player.GetPosition(), 1.0) )
						return false;

					return true;
				}
			}
		}

		return false;
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
	
	override bool AddActionJuncture(ActionData action_data)
	{
		Transport trans = Transport.Cast(action_data.m_Target.GetObject());
		bool accepted = false;
		int nextSeat = trans.CrewPositionIndex( action_data.m_Target.GetComponentIndex() );
		Transport transport = Transport.Cast(action_data.m_Target.GetObject());
		InventoryLocation il = new InventoryLocation;
		if (transport)
		{
			il.SetVehicle(transport, action_data.m_Player, nextSeat);
		
			//Lock target
			if (GetGame().AddInventoryJunctureEx(action_data.m_Player, action_data.m_Player, il, true, 10000))
			{
				accepted = true;
				action_data.m_ReservedInventoryLocations.Insert(il);
			}
		}
		
		return accepted;
	}
};