/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2019-     Equinor ASA
//
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
//
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#include "RicSummaryCaseTools.h"

#include "RimObservedDataCollection.h"
#include "RimObservedSummaryData.h"
#include "RimSummaryCase.h"
#include "RimSummaryCaseCollection.h"
#include "RimSummaryCaseMainCollection.h"

#include "cafPdmObject.h"
#include "cafSelectionManager.h"

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<RimSummaryCase*> RicSummaryCaseTools::selectedSummaryCases()
{
    std::vector<RimSummaryCaseMainCollection*> mainCollectionSelection;
    caf::SelectionManager::instance()->objectsByType( &mainCollectionSelection );

    if ( mainCollectionSelection.size() > 0 )
    {
        return mainCollectionSelection[0]->allSummaryCases();
    }

    std::vector<RimSummaryCase*> caseSelection;
    caf::SelectionManager::instance()->objectsByType( &caseSelection );

    {
        std::vector<RimSummaryCaseCollection*> collectionSelection;
        caf::SelectionManager::instance()->objectsByType( &collectionSelection );

        for ( auto collection : collectionSelection )
        {
            std::vector<RimSummaryCase*> summaryCaseCollection = collection->allSummaryCases();
            caseSelection.insert( caseSelection.end(), summaryCaseCollection.begin(), summaryCaseCollection.end() );
        }
    }

    {
        std::vector<RimObservedDataCollection*> collectionSelection;
        caf::SelectionManager::instance()->objectsByType( &collectionSelection );

        for ( auto collection : collectionSelection )
        {
            std::vector<RimObservedSummaryData*> observedCases = collection->allObservedSummaryData();
            caseSelection.insert( caseSelection.end(), observedCases.begin(), observedCases.end() );
        }
    }

    return caseSelection;
}
